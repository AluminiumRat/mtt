#pragma once

#include <memory>

#include <QtCore/QString>

#include <mtt/application/DrawModel/MasterDrawModel.h>
#include <mtt/application/DrawModel/SlaveDrawModel.h>
#include <mtt/application/ResourceManager/ResourceLibrary.h>
#include <mtt/application/ResourceManager/MMDModelDescription.h>
#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/render/Mesh/MeshTechniquesFactory.h>

namespace mtt
{
  class MMDModelLibrary : public ResourceLibrary< MasterDrawModel,
                                                  MMDModelDescription>
  {
  public:
    MMDModelLibrary(std::unique_ptr<MeshTechniquesFactory> techniqueFactory,
                    Texture2DLibrary& textureLibrary);
    MMDModelLibrary(const MMDModelLibrary&) = delete;
    MMDModelLibrary& operator = (const MMDModelLibrary&) = delete;
    virtual ~MMDModelLibrary() noexcept = default;

    /// Thread safe method
    std::unique_ptr<SlaveDrawModel> load( const QString& filename,
                                          LogicalDevice& device);
 
  protected:
    virtual std::shared_ptr<MasterDrawModel> buildResource(
                              const MMDModelDescription& description) override;

  private:
    std::unique_ptr<MeshTechniquesFactory> _techniqueFactory;
    Texture2DLibrary& _textureLibrary;
  };
};