#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <mtt/application/DrawModel/MMDModelLoader.h>
#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/application/DataStream.h>
#include <mtt/render/Mesh/MeshTechniquesFactory.h>
#include <mtt/render/SceneGraph/Joint.h>

using namespace mtt;

MMDModelLoader::MMDModelLoader( const QString& filename,
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

std::unique_ptr<MasterDrawModel> MMDModelLoader::load()
{
  if (!_fileDirectory.exists()) throw std::runtime_error("The file directory does not exist");

  QFile file(_filename);
  if (!file.open(QFile::ReadOnly)) throw std::runtime_error("Unable to open model file");
  _file = &file;

  mtt::DataStream stream(&file);
  _stream = &stream;

  _checkHead();

  ObjectHeader rootHeader = _loadObjectHeader();
  if(rootHeader.objectTypeIndex != 7) throw std::runtime_error("Wrong type index of root object");

  _loadMaterials();

  std::unique_ptr<MasterDrawModel> model(new MasterDrawModel);
  _model = model.get();

  _loadBones();
  _loadGeometry();
  _loadAnimations();

  return model;
}

void MMDModelLoader::_checkHead()
{
  std::string head;
  head.resize(fileHead.size());
  _file->read(head.data(), head.size());
  if (head != fileHead) throw std::runtime_error("Invalid mmd file header");
  uint32_t fileVersion;
  *_stream >> fileVersion;
  if (fileVersion > lastFileVersion) throw std::runtime_error("Unsupported version of mmd file");
}

QString MMDModelLoader::_loadTextureFilename()
{
  QString relativePath;
  *_stream >> relativePath;
  if (relativePath.isEmpty()) return QString();
  QFileInfo fileInfo(_fileDirectory.absoluteFilePath(relativePath));
  return fileInfo.canonicalFilePath();
}

MMDModelLoader::MaterialData MMDModelLoader::_loadMaterialData()
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

void MMDModelLoader::_loadMaterials()
{
  QString groupName;
  (*_stream) >> groupName;

  uint32_t materialsNumber = _stream->readUint32();
  for (; materialsNumber != 0; materialsNumber--)
  {
    ObjectHeader header = _loadObjectHeader();
    _materialSet[header.id] = _loadMaterialData();
  }
}

MMDModelLoader::ObjectHeader MMDModelLoader::_loadObjectHeader()
{
  ObjectHeader header;
  *_stream >> header.objectTypeIndex >> header.id >> header.name;
  return header;
}

void MMDModelLoader::_loadBones()
{
  QString groupName;
  (*_stream) >> groupName;

  bool visible = _stream->readBool();

  uint32_t boneNumber = _stream->readUint32();
  for (; boneNumber != 0; boneNumber--)
  {
    _loadJoint();
  }
}

Joint& MMDModelLoader::_loadJoint()
{
  ObjectHeader header = _loadObjectHeader();

  TransformTable::Index boneIndex = _model->transformTable().addBone();

  std::unique_ptr<Joint> joint(new Joint);
  Joint& jointRef = *joint;
  _boneSet[header.id] = JointData{ joint.get(), boneIndex };
  _model->addJoint(std::move(joint), boneIndex, header.name);

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

glm::mat4 MMDModelLoader::_loadTransform()
{
  glm::quat rotation = _stream->readQuat();
  glm::vec3 position = _stream->readVec3();
  glm::vec3 scale = _stream->readVec3();

  glm::mat4 transform = glm::translate(glm::mat4(1), position) *
                        glm::mat4_cast(rotation) *
                        glm::scale(glm::mat4(1), scale);
  return transform;
}

void MMDModelLoader::_loadGeometry()
{
  QString groupName;
  (*_stream) >> groupName;

  bool visible = _stream->readBool();

  uint32_t lodsNumber = _stream->readUint32();
  for (; lodsNumber != 0; lodsNumber--)
  {
    ObjectHeader header = _loadObjectHeader();

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

void MMDModelLoader::_loadMesh(bool visible, float minMppx, float maxMppx)
{
  ObjectHeader header = _loadObjectHeader();

  bool localVisible = _stream->readBool();
  visible = visible && localVisible;

  glm::mat4 transform = _loadTransform();

  UID boneId = _stream->readUID();

  mtt::CommonMeshGeometry geometry;
  _loadGeometry(geometry);
  SkinControlNode::BoneRefs boneRefs = _loadBoneRefs();

  UID materialId = _stream->readUID();

  if(visible && geometry.positions.size() != 0)
  {
    std::unique_ptr<Joint> joint(new Joint);
    Joint& jointRef = *joint;
    _model->addJoint(std::move(joint), TransformTable::notIndex, header.name);
    
    jointRef.setJointMatrix(transform);

    BoneSet::iterator iBone = _boneSet.find(boneId);
    if (iBone != _boneSet.end()) iBone->second.joint->addChild(jointRef);

    std::unique_ptr<MeshControlNode> meshNode(new MeshControlNode(_device));
    MeshControlNode& meshRef = *meshNode;
    _model->addMeshNode(std::move(meshNode));

    jointRef.addChild(meshRef);
    meshRef.lodFilter().setRange(minMppx, maxMppx);

    meshRef.setLocalBoundSphere(
                        geometry.calculateBoundingBox().buildBoundingSphere());
    meshRef.mesh().setGeometry(std::move(geometry));

    meshRef.setBoneRefs(boneRefs);

    _adjustMaterial(meshRef, materialId);
    _techniquesFactory.setupTechniques(meshRef.mesh());
  }
}

void MMDModelLoader::_loadGeometry(mtt::CommonMeshGeometry& geometry)
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

SkinControlNode::BoneRefs MMDModelLoader::_loadBoneRefs()
{
  SkinControlNode::BoneRefs result;

  uint16_t boneRefsNumber = _stream->readUint16();
  for (; boneRefsNumber != 0; boneRefsNumber--)
  {
    QString name;
    *_stream >> name;

    UID boneId = _stream->readUID();
    BoneSet::iterator iBone = _boneSet.find(boneId);
    if(iBone == _boneSet.end())
    {
      QString errorString =
                      QString("Unable to find bone % for skin mesh.").arg(name);
      throw std::runtime_error(errorString.toLocal8Bit().data());
    }

    SkinControlNode::BoneRefData boneRef;
    boneRef.joint = iBone->second.joint;
    boneRef.inverseBoneMatrix = _stream->readMat4();

    result.push_back(boneRef);
  }

  return result;
}

void MMDModelLoader::_adjustMaterial(MeshControlNode& meshNode,
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

std::unique_ptr<Sampler> MMDModelLoader::loadTexture(const QString& fileName)
{
  std::unique_ptr<Sampler> sampler(new Sampler( PipelineResource::STATIC,
                                                _device));
  std::shared_ptr<Texture2D> texture =
                                  _textureLibrary.load(fileName, _device, true);
  sampler->setAttachedTexture(texture);
  return sampler;
}

void MMDModelLoader::_loadAnimations()
{
  QString groupName;
  (*_stream) >> groupName;

  uint32_t animationsNumber = _stream->readUint32();
  for (; animationsNumber != 0; animationsNumber--)
  {
    ObjectHeader header = _loadObjectHeader();

    std::unique_ptr<DrawModelAnimation> animation(new DrawModelAnimation());

    uint32_t tracksNumber = _stream->readUint32();
    for (; tracksNumber != 0; tracksNumber--)
    {
      std::unique_ptr<DrawModelAnimationTrack> track = loadAnimationTrack();
      if(track != nullptr) animation->addTrack(std::move(track));
    }

    _model->addAnimation(std::move(animation), header.name);
  }
}

template<typename ValueType>
void MMDModelLoader::_readKeypoint(
                                mtt::ValueKeypoint<ValueType, TimeT>& keypoint)
{
  uint32_t timeCount = _stream->readUint32();
  TimeT time(timeCount);
  keypoint.setTime(time);

  ValueType value;
  *_stream >> value;
  keypoint.setValue(value);

  uint8_t interpolation = _stream->readInt8();
  keypoint.setInterpolation(mtt::InterpolationType(interpolation));
}

std::unique_ptr<DrawModelAnimationTrack> MMDModelLoader::loadAnimationTrack()
{
  ObjectHeader header = _loadObjectHeader();

  bool trackEnabled = _stream->readBool();
  std::unique_ptr<DrawModelAnimationTrack> track;
  if(trackEnabled) track.reset(new DrawModelAnimationTrack);

  uint16_t keypointsNumber = _stream->readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<DrawModelAnimationTrack::PositionKeypoint> keypoint(
                              new DrawModelAnimationTrack::PositionKeypoint());
    _readKeypoint(*keypoint);
    if (trackEnabled) track->addPositionKeypoint(std::move(keypoint));
  }

  keypointsNumber = _stream->readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<DrawModelAnimationTrack::RotationKeypoint> keypoint(
                              new DrawModelAnimationTrack::RotationKeypoint());
    _readKeypoint(*keypoint);
    if (trackEnabled) track->addRotationKeypoint(std::move(keypoint));
  }

  keypointsNumber = _stream->readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<DrawModelAnimationTrack::ScaleKeypoint> keypoint(
                                        new DrawModelAnimationTrack::ScaleKeypoint());
    _readKeypoint(*keypoint);
    if (trackEnabled) track->addScaleKeypoint(std::move(keypoint));
  }

  UID boneId = _stream->readUID();
  if(trackEnabled)
  {
    BoneSet::iterator iBone = _boneSet.find(boneId);
    if(iBone != _boneSet.end()) track->setBoneIndex(iBone->second.boneIndex);
    else track->setBoneIndex(TransformTable::notIndex);
  }

  return track;
}
