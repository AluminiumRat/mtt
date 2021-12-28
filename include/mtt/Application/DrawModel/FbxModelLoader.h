#pragma once

#include <memory>
#include <vector>

#include <QtCore/QString>

#include <mtt/Application/DrawModel/MasterDrawModel.h>
#include <mtt/Fbx/BaseFbxImporter.h>
#include <mtt/Render/Pipeline/Sampler.h>

namespace mtt
{
  class LogicalDevice;
  class MeshTechniquesFactory;
  class Texture2DLibrary;

  class FbxModelLoader : private BaseFbxImporter
  {
  public:
    FbxModelLoader( const QString& filename,
                    BaseFbxImporter::MaterialOptions materialOptions,
                    MeshTechniquesFactory& techniqueFactory,
                    Texture2DLibrary& textureLibrary,
                    LogicalDevice& device);
    FbxModelLoader(const FbxModelLoader&) = delete;
    FbxModelLoader& operator = (const FbxModelLoader&) = delete;
    virtual ~FbxModelLoader() noexcept = default;

    std::unique_ptr<MasterDrawModel> load();

  protected:
    virtual void pushTranslation(FbxNode& node) override;
    virtual void popTranslation() override;
    virtual void processMesh( mtt::CommonMeshGeometry&& vertices,
                              const FbxSurfaceMaterial& fbxMaterial,
                              const Bones& bones,
                              const FbxMesh& mesh) override;

  private:
    std::unique_ptr<Sampler> loadTexture(const QString& fileName);

  private:
    QString _filename;
    BaseFbxImporter::MaterialOptions _materialOptions;
    MeshTechniquesFactory& _techniquesFactory;
    Texture2DLibrary& _textureLibrary;
    LogicalDevice& _device;

    MasterDrawModel* _model;

    std::vector<Joint*> _jointsStack;
  };
}