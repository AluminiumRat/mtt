#pragma once

#include <memory>
#include <vector>

#include <QtCore/QString>

#include <mtt/application/DrawModel/MasterDrawModel.h>
#include <mtt/fbx/BaseFbxImporter.h>
#include <mtt/render/Pipeline/Sampler.h>

namespace mtt
{
  class LogicalDevice;
  class MeshExtraData;
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
    virtual void processScene(FbxScene& scene) override;
    virtual void pushTranslation(FbxNode& node) override;
    virtual void popTranslation() override;
    virtual void processMesh( mtt::CommonMeshGeometry&& vertices,
                              const FbxSurfaceMaterial& fbxMaterial,
                              const Bones& bones,
                              const FbxMesh& mesh) override;

  private:
    struct JointRecord
    {
      Joint* joint = nullptr;
      TransformTable::Index boneIndex = TransformTable::notIndex;
    };

    JointRecord _getOrCreateJoint(FbxNode& node);
    void loadAnimationTrack(FbxNode& node,
                            TransformTable::Index boneIndex);

    void _updateMaterial( MeshExtraData& extraData,
                          const FbxSurfaceMaterial& fbxMaterial);
    std::unique_ptr<Sampler> _loadTexture(const QString& fileName);

  private:
    QString _filename;
    BaseFbxImporter::MaterialOptions _materialOptions;
    MeshTechniquesFactory& _techniquesFactory;
    Texture2DLibrary& _textureLibrary;
    LogicalDevice& _device;

    FbxAnimLayer* _animationLayer;

    MasterDrawModel* _model;
    DrawModelAnimation* _animation;

    using JointsStack = std::vector<Joint*>;
    JointsStack _jointsStack;

    using JointsMap = std::map<FbxNode*, JointRecord>;
    JointsMap _jointsMap;
  };
}