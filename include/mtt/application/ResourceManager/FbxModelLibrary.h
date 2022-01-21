#pragma once

#include <memory>

#include <QtCore/QString>

#include <mtt/application/DrawModel/MasterDrawModel.h>
#include <mtt/application/DrawModel/SlaveDrawModel.h>
#include <mtt/application/ResourceManager/ResourceLibrary.h>
#include <mtt/application/ResourceManager/FbxModelDescription.h>
#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/render/Mesh/MeshTechniquesFactory.h>

namespace mtt
{
  class FbxModelLibrary : public ResourceLibrary< MasterDrawModel,
                                                  FbxModelDescription>
  {
  public:
    FbxModelLibrary(std::unique_ptr<MeshTechniquesFactory> techniqueFactory,
                    Texture2DLibrary& textureLibrary);
    FbxModelLibrary(const FbxModelLibrary&) = delete;
    FbxModelLibrary& operator = (const FbxModelLibrary&) = delete;
    virtual ~FbxModelLibrary() noexcept = default;

    /// Thread safe method
    std::unique_ptr<SlaveDrawModel> load(
                              const QString& filename,
                              BaseFbxImporter::MaterialOptions materialOptions,
                              LogicalDevice& device);
 
  protected:
    virtual std::shared_ptr<MasterDrawModel> buildResource(
                              const FbxModelDescription& description) override;

  private:
    std::unique_ptr<MeshTechniquesFactory> _techniqueFactory;
    Texture2DLibrary& _textureLibrary;
  };
};