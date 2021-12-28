#pragma once

#include <memory>

#include <QtCore/QString>

#include <mtt/Application/DrawModel/MasterDrawModel.h>
#include <mtt/Application/DrawModel/SlaveDrawModel.h>
#include <mtt/Application/ResourceManager/ResourceLibrary.h>
#include <mtt/Application/ResourceManager/FbxModelDescription.h>
#include <mtt/Application/ResourceManager/Texture2DLibrary.h>
#include <mtt/Render/Mesh/MeshTechniquesFactory.h>

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