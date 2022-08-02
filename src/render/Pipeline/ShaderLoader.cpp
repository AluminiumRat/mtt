#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <mtt/render/Pipeline/ShaderLoader.h>

using namespace mtt;

std::string ShaderLoader::loadText(const std::string& filename) const
{
  std::string text = loadDefault(filename);
  if(text.empty()) throw std::runtime_error("ShaderLoader: Failed to read shader file :" + filename);
  return text;
}

std::string ShaderLoader::loadDefault(
                                    const std::string& filename) const noexcept
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
  if (!file.is_open()) return std::string();

  size_t dataSize = (size_t)file.tellg();
  std::string text;
  text.resize(dataSize);

  file.seekg(0);
  file.read(text.data(), dataSize);

  return text;
}
