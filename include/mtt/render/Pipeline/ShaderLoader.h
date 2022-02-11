#pragma once

#include <string>

namespace mtt
{
  class ShaderLoader
  {
  public:
    ShaderLoader() = default;
    ShaderLoader(const ShaderLoader&) = delete;
    ShaderLoader& operator = (const ShaderLoader&) = delete;
    virtual ~ShaderLoader() noexcept = default;

    /// filename in utf8 encoding
    virtual std::string loadText(const std::string& filename) const;

  protected:
    /// returns empty string if file not found or not available
    std::string loadDefault(const std::string& filename) const noexcept;
  };
}