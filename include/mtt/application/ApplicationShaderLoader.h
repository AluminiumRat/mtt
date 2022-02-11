#pragma once

#include <mtt/render/Pipeline/ShaderLoader.h>

namespace mtt
{
  class ApplicationShaderLoader : public ShaderLoader
  {
  public:
    ApplicationShaderLoader() = default;
    ApplicationShaderLoader(const ApplicationShaderLoader&) = delete;
    ApplicationShaderLoader& operator = (
                                      const ApplicationShaderLoader&) = delete;
    virtual ~ApplicationShaderLoader() noexcept = default;

    /// filename in utf8 encoding
    virtual std::string loadText(const std::string& filename) const override;

  protected:
    /// return empty string if file is not available
    std::string loadQt(const std::string& filename) const noexcept;
  };
}