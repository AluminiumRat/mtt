#pragma once

#include <QtCore/QString>

#include <mtt/application/ResourceManager/ResourceLibrary.h>
#include <mtt/application/ResourceManager/Texture2DDescription.h>

#include <mtt/render/Pipeline/Texture2D.h>

namespace mtt
{
  class Texture2DLibrary : public ResourceLibrary<Texture2D,
                                                  Texture2DDescription>
  {
  public:
    Texture2DLibrary() = default;
    Texture2DLibrary(const Texture2DLibrary&) = delete;
    Texture2DLibrary& operator = (const Texture2DLibrary&) = delete;
    virtual ~Texture2DLibrary() noexcept = default;

    /// Thread safe method
    std::shared_ptr<Texture2D> load(const QString& filename,
                                    LogicalDevice& device,
                                    bool generateLods);
 
  protected:
    virtual std::shared_ptr<Texture2D> buildResource(
                              const Texture2DDescription& description) override;
  };
};