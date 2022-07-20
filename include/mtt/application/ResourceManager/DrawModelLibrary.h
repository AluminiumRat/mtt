#pragma once

#include <memory>

#include <QtCore/QString>

#include <mtt/application/DrawModel/MasterDrawModel.h>
#include <mtt/application/DrawModel/SlaveDrawModel.h>
#include <mtt/application/ResourceManager/ResourceLibrary.h>
#include <mtt/application/ResourceManager/DrawModelDescription.h>
#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/render/Mesh/MeshTechniquesFactory.h>

namespace mtt
{
  class DrawModelLibrary : public ResourceLibrary<MasterDrawModel,
                                                  DrawModelDescription>
  {
  public:
    DrawModelLibrary( std::unique_ptr<MeshTechniquesFactory> techniqueFactory,
                      Texture2DLibrary& textureLibrary);
    DrawModelLibrary(const DrawModelLibrary&) = delete;
    DrawModelLibrary& operator = (const DrawModelLibrary&) = delete;
    virtual ~DrawModelLibrary() noexcept = default;

    /// Thread safe method
    std::unique_ptr<SlaveDrawModel> load( const QString& filename,
                                          LogicalDevice& device);
 
  protected:
    virtual std::shared_ptr<MasterDrawModel> buildResource(
                            const DrawModelDescription& description) override;

  private:
    std::unique_ptr<MeshTechniquesFactory> _techniqueFactory;
    Texture2DLibrary& _textureLibrary;
  };
};