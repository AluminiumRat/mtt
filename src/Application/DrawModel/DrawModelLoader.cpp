#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <mtt/Application/DrawModel/DrawModelLoader.h>
#include <mtt/Application/ResourceManager/Texture2DLibrary.h>
#include <mtt/Application/DataStream.h>
#include <mtt/Render/Mesh/MeshTechniquesFactory.h>
#include <mtt/Render/SceneGraph/Joint.h>

using namespace mtt;

DrawModelLoader::DrawModelLoader( const QString& filename,
                                  MeshTechniquesFactory& techniqueFactory,
                                  Texture2DLibrary& textureLibrary,
                                  LogicalDevice& device) :
  _filename(filename),
  _techniquesFactory(techniqueFactory),
  _textureLibrary(textureLibrary),
  _file(nullptr),
  _stream(nullptr),
  _fileDirectory(QFileInfo(_filename).dir()),
  _device(device),
  _model(nullptr)
{
}

std::unique_ptr<DrawModel> DrawModelLoader::load()
{
  if (!_fileDirectory.exists()) throw std::runtime_error("The file directory does not exist");

  QFile file(_filename);
  if (!file.open(QFile::ReadOnly)) throw std::runtime_error("Unable to open model file");
  _file = &file;

  mtt::DataStream stream(&file);
  _stream = &stream;

  _checkHead();
  _loadMaterials();

  std::unique_ptr<DrawModel> model(new DrawModel);
  _model = model.get();

  _loadBones();
  _loadGeometry();

  return model;
}

void DrawModelLoader::_checkHead()
{
  std::string head;
  head.resize(fileHead.size());
  _file->read(head.data(), head.size());
  if (head != fileHead) throw std::runtime_error("Invalid mmd file header");
  uint32_t fileVersion;
  *_stream >> fileVersion;
  if (fileVersion > lastFileVersion) throw std::runtime_error("Unsupported version of mmd file");
}

QString DrawModelLoader::_loadTextureFilename()
{
  QString relativePath;
  *_stream >> relativePath;
  if (relativePath.isEmpty()) return QString();
  QFileInfo fileInfo(_fileDirectory.absoluteFilePath(relativePath));
  return fileInfo.canonicalFilePath();
}

DrawModelLoader::MaterialData DrawModelLoader::_loadMaterialData()
{
  MaterialData material;
  material.albedo = _stream->readVec3();
  material.albedoTexture = _loadTextureFilename();
  material.useAlphaFromAlbedoTexture = _stream->readBool();

  material.roughness = _stream->readFloat();
  material.specularStrength = _stream->readFloat();
  material.specularTexture = _loadTextureFilename();

  material.metallic = _stream->readFloat();

  material.opaque = _stream->readFloat();
  material.opaqueTexture = _loadTextureFilename();

  material.reflectionFactor = _stream->readFloat();
  material.reflectionTexture = _loadTextureFilename();

  material.emission = _stream->readVec3();
  material.emission *= _stream->readFloat();
  material.emissionTexture = _loadTextureFilename();

  material.normalTexture = _loadTextureFilename();

  return material;
}

void DrawModelLoader::_loadMaterials()
{
  uint32_t materialsNumber = _stream->readUint32();
  for (; materialsNumber != 0; materialsNumber--)
  {
    uint16_t objectTypeIndex;
    *_stream >> objectTypeIndex;

    UID id = _stream->readUID();

    QString name;
    *_stream >> name;

    _materialSet[id] = _loadMaterialData();
  }
}

void DrawModelLoader::_loadBones()
{
  uint32_t boneNumber = _stream->readUint32();
  for (; boneNumber != 0; boneNumber--)
  {
    _loadJoint();
  }
}

Joint& DrawModelLoader::_loadJoint()
{
  uint16_t objectTypeIndex;
  *_stream >> objectTypeIndex;

  UID id = _stream->readUID();

  QString name;
  *_stream >> name;

  DrawModelTransformTable::Index boneIndex =
                                        _model->transformTable().addBone(name);

  std::unique_ptr<Joint> joint(new Joint);
  Joint& jointRef = *joint;
  _boneSet[id] = JointData{ joint.get(), boneIndex };
  _model->addJoint(std::move(joint), boneIndex);

  _stream->readBool(); //visible
  glm::mat4 transform = _loadTransform();
  jointRef.setJointMatrix(transform);
  _model->transformTable().setTransform(boneIndex, transform);

  uint32_t childNumber = _stream->readUint32();
  for (; childNumber != 0; childNumber--)
  {
    Joint& child = _loadJoint();
    jointRef.addChild(child);
  }

  return jointRef;
}

glm::mat4 DrawModelLoader::_loadTransform()
{
  glm::quat rotation = _stream->readQuat();
  glm::vec3 position = _stream->readVec3();
  glm::vec3 scale = _stream->readVec3();

  glm::mat4 transform = glm::translate(glm::mat4(1), position) *
                        glm::mat4_cast(rotation) *
                        glm::scale(glm::mat4(1), scale);
  return transform;
}

void DrawModelLoader::_loadGeometry()
{
  uint32_t lodsNumber = _stream->readUint32();
  for (; lodsNumber != 0; lodsNumber--)
  {
    uint16_t objectTypeIndex;
    *_stream >> objectTypeIndex;

    UID id = _stream->readUID();

    QString name;
    *_stream >> name;

    bool visible = _stream->readBool();
    float minMppx = _stream->readFloat();
    float maxMppx = _stream->readFloat();

    uint32_t childNumber = _stream->readUint32();
    for (; childNumber != 0; childNumber--)
    {
      _loadMesh(visible, minMppx, maxMppx);
    }
  }
}

void DrawModelLoader::_loadMesh(bool visible, float minMppx, float maxMppx)
{
  uint16_t objectTypeIndex;
  *_stream >> objectTypeIndex;

  UID id = _stream->readUID();

  QString name;
  *_stream >> name;

  bool localVisible = _stream->readBool();
  visible = visible && localVisible;

  glm::mat4 transform = _loadTransform();

  UID boneId = _stream->readUID();

  mtt::CommonMeshGeometry geometry;
  _loadGeometry(geometry);
  _loadBoneRefs();

  UID materialId = _stream->readUID();

  if(visible && geometry.positions.size() != 0)
  {
    std::unique_ptr<Joint> joint(new Joint);
    Joint& jointRef = *joint;
    _model->addJoint(std::move(joint), DrawModelTransformTable::notIndex);
    
    jointRef.setJointMatrix(transform);

    BoneSet::iterator iBone = _boneSet.find(boneId);
    if (iBone != _boneSet.end()) iBone->second.joint->addChild(jointRef);

    std::unique_ptr<DrawModelMeshNode> meshNode(new DrawModelMeshNode(_device));
    DrawModelMeshNode& meshRef = *meshNode;
    _model->addMeshNode(std::move(meshNode));

    jointRef.addChild(meshRef);
    meshRef.lodFilter().setRange(minMppx, maxMppx);

    meshRef.setLocalBoundSphere(
                        geometry.calculateBoundingBox().buildBoundingSphere());
    meshRef.mesh().setGeometry(std::move(geometry));

    _adjustMaterial(meshRef, materialId);
    _techniquesFactory.setupTechniques(meshRef.mesh());
  }
}

void DrawModelLoader::_loadGeometry(mtt::CommonMeshGeometry& geometry)
{
  *_stream >> geometry.positions;
  *_stream >> geometry.normals;
  *_stream >> geometry.tangents;
  *_stream >> geometry.binormals;
  *_stream >> geometry.texCoords;
  uint64_t skeletonRefsNumber = _stream->readUint64();
  geometry.skeletonRefs.resize(skeletonRefsNumber);
  for (mtt::CommonMeshGeometry::SkeletonRef& skeletonRef :
                                                        geometry.skeletonRefs)
  {
    *_stream >> skeletonRef.bonesNumber;
    for (uint16_t boneIndex = 0;
      boneIndex < skeletonRef.bonesNumber;
      boneIndex++)
    {
      *_stream >> skeletonRef.bones[boneIndex].boneIndex;
      *_stream >> skeletonRef.bones[boneIndex].weight;
    }
  }

  *_stream >> geometry.triangleIndices;
  *_stream >> geometry.lineIndices;
}

void DrawModelLoader::_loadBoneRefs()
{
  uint16_t boneRefsNumber = _stream->readUint16();
  for (; boneRefsNumber != 0; boneRefsNumber--)
  {
    QString name;
    *_stream >> name;

    UID boneId = _stream->readUID();
    glm::mat4 boneInverseMatrix = _stream->readMat4();
  }
}

void DrawModelLoader::_adjustMaterial(DrawModelMeshNode& meshNode,
                                      UID materialId)
{
  MaterialSet::const_iterator iMaterial = _materialSet.find(materialId);
  if(iMaterial == _materialSet.end()) return;
  const MaterialData& materialData = iMaterial->second;

  MeshExtraData& meshExtraData = meshNode.mesh().extraData();

  meshExtraData.setSurfaceMaterialData(materialData);

  if(!materialData.albedoTexture.isEmpty())
  {
    meshExtraData.setAlbedoSampler(loadTexture(materialData.albedoTexture));
  }
  meshExtraData.setAlphaInAlbedoSamplerIsOpacity(
                                        materialData.useAlphaFromAlbedoTexture);

  if (!materialData.opaqueTexture.isEmpty())
  {
    meshExtraData.setOpaqueSampler(loadTexture(materialData.opaqueTexture));
  }

  if (!materialData.specularTexture.isEmpty())
  {
    meshExtraData.setSpecularSampler(loadTexture(materialData.specularTexture));
  }

  if (!materialData.normalTexture.isEmpty())
  {
    meshExtraData.setNormalSampler(loadTexture(materialData.normalTexture));
  }

  if (!materialData.emissionTexture.isEmpty())
  {
    meshExtraData.setEmissionSampler(loadTexture(materialData.emissionTexture));
  }

  if (!materialData.reflectionTexture.isEmpty())
  {
    meshExtraData.setReflectionSampler(
                                  loadTexture(materialData.reflectionTexture));
  }
}

std::unique_ptr<Sampler> DrawModelLoader::loadTexture(const QString& fileName)
{
  std::unique_ptr<Sampler> sampler(new Sampler( PipelineResource::STATIC,
                                                _device));
  std::shared_ptr<Texture2D> texture =
                                  _textureLibrary.load(fileName, _device, true);
  sampler->setAttachedTexture(texture);
  return sampler;
}
