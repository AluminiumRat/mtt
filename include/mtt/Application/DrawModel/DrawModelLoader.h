#pragma once

#include <memory>
#include <string>

#include <glm/mat4x4.hpp>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/Application/DrawModel/DrawModel.h>
#include <mtt/Application/DrawModel/DrawModelMeshNode.h>
#include <mtt/Render/Pipeline/Sampler.h>
#include <mtt/Utilities/SurfaceMaterialData.h>
#include <mtt/Utilities/UID.h>

class QFile;

namespace mtt
{
  class DataStream;
  class MeshTechniquesFactory;
  class Texture2DLibrary;
  class Joint;

  class DrawModelLoader
  {
  public:
    static const inline std::string fileHead = "MMDModelFile";
    static constexpr uint32_t lastFileVersion = 0;

  public:
    DrawModelLoader(const QString& filename,
                    MeshTechniquesFactory& techniqueFactory,
                    Texture2DLibrary& textureLibrary,
                    LogicalDevice& device);
    DrawModelLoader(const DrawModelLoader&) = delete;
    DrawModelLoader& operator = (const DrawModelLoader&) = delete;
    virtual ~DrawModelLoader() noexcept = default;

    std::unique_ptr<DrawModel> load();

  private:
    struct MaterialData : public SurfaceMaterialData
    {
      QString albedoTexture;
      bool useAlphaFromAlbedoTexture;
      QString opaqueTexture;
      QString specularTexture;
      QString normalTexture;
      QString emissionTexture;
      QString reflectionTexture;
    };

    struct JointData
    {
      Joint* joint;
      DrawModelTransformTable::Index boneIndex;
    };

  private:
    void _checkHead();
    QString _loadTextureFilename();
    MaterialData _loadMaterialData();
    void _loadMaterials();
    Joint& _loadJoint();
    void _loadBones();
    glm::mat4 _loadTransform();
    void _loadGeometry();
    void _loadMesh(bool visible, float minMppx, float maxMppx);
    void _loadGeometry(mtt::CommonMeshGeometry& geometry);
    DrawModelMeshNode::BoneRefs _loadBoneRefs();
    void _adjustMaterial(DrawModelMeshNode& meshNode, UID materialId);
    std::unique_ptr<Sampler> loadTexture(const QString& fileName);

  private:
    QString _filename;
    MeshTechniquesFactory& _techniquesFactory;
    Texture2DLibrary& _textureLibrary;
    QFile* _file;
    mtt::DataStream* _stream;
    QDir _fileDirectory;

    LogicalDevice& _device;

    DrawModel* _model;

    using MaterialSet = std::map<UID, MaterialData>;
    MaterialSet _materialSet;

    using BoneSet = std::map<UID, JointData>;
    BoneSet _boneSet;
  };
}