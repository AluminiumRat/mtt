#pragma once

#include <QtCore/QString>
#include <QtGui/QImage>

#include <mtt/application/ResourceManager/ResourceLibrary.h>
#include <mtt/application/ResourceManager/CubeTextureDescription.h>
#include <mtt/render/Pipeline/CubeTexture.h>

namespace mtt
{
  class CubeTextureLibrary : public ResourceLibrary<CubeTexture,
                                                    CubeTextureDescription>
  {
  public:
    CubeTextureLibrary() = default;
    CubeTextureLibrary(const CubeTextureLibrary&) = delete;
    CubeTextureLibrary& operator = (const CubeTextureLibrary&) = delete;
    virtual ~CubeTextureLibrary() noexcept = default;

    /// Thread safe method
    std::shared_ptr<CubeTexture> load(std::array<QString, 6> filenames,
                                      LogicalDevice& device,
                                      bool generateLods);

    /// Load texture without adding to library
    static std::unique_ptr<CubeTexture> loadTexture(
                                              std::array<QString, 6> filenames,
                                              LogicalDevice& device,
                                              bool generateLods);

  protected:
    virtual std::shared_ptr<CubeTexture> buildResource(
                           const CubeTextureDescription& description) override;

  private:
    static std::array<QImage, 6> _readData(std::array<QString, 6> filenames);
    static uint _getExtent(const std::array<QImage, 6>& images);
    static void _checkExtent(uint extent, std::array<QImage, 6>& images);
  };
};