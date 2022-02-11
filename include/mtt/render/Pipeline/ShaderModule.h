#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

namespace mtt
{
  class LogicalDevice;

  class ShaderModule
  {
  public:
    enum Type
    {
      VERTEX_SHADER,
      FRAGMENT_SHADER,
      COMPUTE_SHADER,
      GEOMETRY_SHADER,
      TESSELLATION_CONTROL_SHADER,
      TESSELLATION_EVALUATION_SHADER
    };

  public:
    class Fragment
    {
    private:
      friend class ShaderModule;
      explicit Fragment(ShaderModule& shaderModule) noexcept;
    public:
      Fragment(const Fragment&) = delete;
      Fragment operator = (const Fragment&) = delete;
      ~Fragment() noexcept = default;

    public:
      inline const std::string& text() const noexcept;
      inline const std::string& hash() const noexcept;
      void setText( const std::string& newText,
                    const std::string& newHash = std::string());

      /// filename in utf8 encoding
      void loadFromFile(const std::string& filename);
      void replace( const std::string& originWord,
                    const std::string& newWord);

    private:
      std::string _text;
      std::string _hash;
      ShaderModule& _shaderModule;
    };

  public:
    ShaderModule(Type theType, LogicalDevice& device);
    ShaderModule(const ShaderModule&) = delete;
    ShaderModule& operator = (const ShaderModule&) = delete;
    virtual ~ShaderModule() noexcept = default;

    inline Type type() const noexcept;

    inline LogicalDevice& device() const noexcept;
    /// Rebuild module if needs and returns handle. If no text and no uploads
    /// from file then VK_NULL_HANDLE returns.
    inline VkShaderModule handle();

    /// Revision starts from 0. 0 is value in invalid state.
    /// Revision will be incremented with every shader change.
    inline size_t revision() const noexcept;

    Fragment& newFragment();
    void removeFragment(Fragment& fragment) noexcept;

    void setDefine(const std::string& name, const std::string& value);
    void setDefine(const std::string& name);
    inline const std::string* defineValue(const std::string& name) noexcept;
    void removeDefine(const std::string& name) noexcept;

  private:
    void _cleanup() noexcept;
    void _incrementRevision() noexcept;
    void _rebuild();
    VkShaderModule _compile(const std::string& code);
    VkShaderModule _buildFromSPIR(const uint32_t* data, size_t datasize);

  private:
    Type _type;

    LogicalDevice& _device;
    VkShaderModule _handle;
    size_t _revision;

    using Fragments = std::vector<std::unique_ptr<Fragment>>;
    Fragments _fragments;

    using Defines = std::map<std::string, std::string>;
    Defines _defines;
  };

  inline const std::string& ShaderModule::Fragment::text() const noexcept
  {
    return _text;
  }

  inline const std::string& ShaderModule::Fragment::hash() const noexcept
  {
    return _hash;
  }

  inline ShaderModule::Type ShaderModule::type() const noexcept
  {
    return _type;
  }

  inline LogicalDevice& ShaderModule::device() const noexcept
  {
    return _device;
  }

  inline VkShaderModule ShaderModule::handle()
  {
    if(_handle == VK_NULL_HANDLE) _rebuild();
    return _handle;
  }

  inline size_t ShaderModule::revision() const noexcept
  {
    return _revision;
  }

  inline const std::string* ShaderModule::defineValue(
                                              const std::string& name) noexcept
  {
    Defines::const_iterator iDefine = _defines.find(name);
    if(iDefine == _defines.end()) return nullptr;
    return &iDefine->second;
  }
}