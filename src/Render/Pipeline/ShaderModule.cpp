#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <shaderc/shaderc.hpp>

#include <mtt/Render/Pipeline/ShaderModule.h>
#include <mtt/Render/LogicalDevice.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

static const std::string shaderHeader = "#version 450";

ShaderModule::Fragment::Fragment(ShaderModule& shaderModule) noexcept :
  _shaderModule(shaderModule)
{
}

void ShaderModule::Fragment::setText( const std::string& newText,
                                      const std::string& newHash)
{
  _text = newText;
  _hash = newHash;
  if(_hash.empty()) _hash = "n";
  _shaderModule._cleanup();
  _shaderModule._incrementRevision();
}

void ShaderModule::Fragment::loadFromFile(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);
  if (!file.is_open())
  {
    file.open("shaders/" + filename, std::ios::ate | std::ios::binary);
  }
  if (!file.is_open())
  {
    const char* patchesEnv = std::getenv("SHADER_SEARCH_PATCHES");
    if(patchesEnv != nullptr)
    {
      std::istringstream envStream(patchesEnv);
      std::string path;
      while(std::getline(envStream, path, ';'))
      {
        file.open(path + "/" + filename, std::ios::ate | std::ios::binary);
        if (file.is_open()) break;
      }
    }
  }
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open shader file :" + filename);
  }

  size_t dataSize = (size_t)file.tellg();
  std::string text;
  text.resize(dataSize);

  file.seekg(0);
  file.read(text.data(), dataSize);

  file.close();

  setText(text, filename);
}

ShaderModule::ShaderModule(Type theType, LogicalDevice& device) :
  _type(theType),
  _device(device),
  _handle(VK_NULL_HANDLE),
  _revision(0)
{
}

void ShaderModule::_cleanup() noexcept
{
  _handle = VK_NULL_HANDLE;
}

void ShaderModule::_incrementRevision() noexcept
{
  _revision++;
}

void ShaderModule::_rebuild()
{
  _cleanup();
  if(_fragments.empty()) return;

  std::string searchString;  
  std::string code(shaderHeader);
  for(std::unique_ptr<Fragment>& fragment : _fragments)
  {
    searchString += fragment->hash();

    code = code + '\n';
    code = code + fragment->text();
  }

  for(Defines::iterator iDefine = _defines.begin();
      iDefine != _defines.end();
      iDefine++)
  {
    searchString += iDefine->first + iDefine->second;
  }
  searchString += code;
  _handle = _device.shaderCache().find(_type, searchString);
  if(_handle != VK_NULL_HANDLE) return;

  VkShaderModule newHandle = _compile(code);

  ShaderCache::ModuleObject moduleObject(_device, newHandle);
  _handle = _device.shaderCache().add(_type,
                                      searchString,
                                      std::move(moduleObject));
}

VkShaderModule ShaderModule::_compile(const std::string& code)
{
  shaderc::Compiler compiler;
  
  shaderc::CompileOptions options;
  options.AddMacroDefinition("M_PI", "3.1415926535897932384626433832795f");
  for(Defines::iterator iDefine = _defines.begin();
      iDefine != _defines.end();
      iDefine++)
  {
    options.AddMacroDefinition(iDefine->first, iDefine->second);
  }

  shaderc_shader_kind kind;
  switch(_type)
  {
    case VERTEX_SHADER:
      kind = shaderc_vertex_shader;
      break;
    case FRAGMENT_SHADER:
      kind = shaderc_fragment_shader;
      break;
    case COMPUTE_SHADER:
      kind = shaderc_compute_shader;
      break;
    case GEOMETRY_SHADER:
      kind = shaderc_geometry_shader;
      break;
    case TESSELLATION_CONTROL_SHADER:
      kind = shaderc_tess_control_shader;
      break;
    case TESSELLATION_EVALUATION_SHADER:
      kind = shaderc_tess_evaluation_shader;
      break;
  }

  shaderc::SpvCompilationResult compilationResult =
                                  compiler.CompileGlslToSpv(code.data(),
                                                            code.size(),
                                                            kind,
                                                            "GeneratedShader",
                                                            options);
  if (compilationResult.GetCompilationStatus() !=
                                            shaderc_compilation_status_success)
  {
    Log() << compilationResult.GetErrorMessage();

    std::stringstream stream(code);
    size_t lineIndex = 1;
    std::string line;
    while(std::getline(stream, line))
    {
      Log() << lineIndex << " : " << line;
      lineIndex++;
    }

    Log() << "Defines:";
    for ( Defines::iterator iDefine = _defines.begin();
          iDefine != _defines.end();
          iDefine++)
    {
      Log() << iDefine->first << " -> " << iDefine->second;
    }

    Abort("ShaderModule::_rebuild: unable to compile shader.");
  }

  std::vector<uint32_t> spirCode( compilationResult.cbegin(),
                                  compilationResult.cend());
  return _buildFromSPIR(spirCode.data(), spirCode.size() * 4);
}

VkShaderModule ShaderModule::_buildFromSPIR(const uint32_t* data,
                                            size_t datasize)
{
  VkShaderModule newHandle;
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = datasize;
  createInfo.pCode = data;
  if (vkCreateShaderModule( _device.handle(),
                            &createInfo,
                            nullptr,
                            &newHandle) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create shader module.");
  }
  return newHandle;
}

ShaderModule::Fragment& ShaderModule::newFragment()
{
  _fragments.emplace_back(new Fragment(*this));
  _cleanup();
  _incrementRevision();
  return *_fragments.back();
}

void ShaderModule::removeFragment(ShaderModule::Fragment& fragment) noexcept
{
  for(Fragments::iterator iFragment = _fragments.begin();
      iFragment != _fragments.end();
      iFragment++)
  {
    if(iFragment->get() == &fragment)
    {
      _fragments.erase(iFragment);
      _cleanup();
      _incrementRevision();
      return;
    }
  }

  Abort("ShaderModule::removeFragment: fragment was not found in shader module.");
}

void ShaderModule::setDefine(const std::string& name, const std::string& value)
{
  Defines::iterator iDefine = _defines.find(name);
  if(iDefine != _defines.end())
  {
    if(iDefine->second != value)
    {
      iDefine->second = value;
      _cleanup();
      _incrementRevision();
    }
  }
  else
  {
    _defines[name] = value;
    _cleanup();
    _incrementRevision();
  }
}

void ShaderModule::setDefine(const std::string& name)
{
  setDefine(name, "");
}

void ShaderModule::removeDefine(const std::string& name) noexcept
{
  try
  {
    _defines.erase(name);
  }
  catch(...)
  {
    Abort("ShaderModule::removeDefine: unable to erace define from define map.");
  }

  _cleanup();
  _incrementRevision();
}
