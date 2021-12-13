#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

#include <AsyncTasks/ObjectDataLoader.h>

ObjectDataLoader::ObjectDataLoader( mtt::DataStream& stream,
                                    const QDir& fileDirectory) :
  _stream(stream),
  _fileDirectory(fileDirectory)
{
}

void ObjectDataLoader::loadObject(mtt::Object& object,
                                  mtt::DataStream& stream,
                                  const QDir& fileDirectory)
{
  ObjectDataLoader loader(stream, fileDirectory);
  loader.process(object);
}

void ObjectDataLoader::visit(AnimationObject& object)
{
  OEVisitor::visit(object);
  uint32_t childNumber = _stream.readUint32();
  for (; childNumber != 0; childNumber--)
  {
    object.addChild(loadObject<AnimationTrack>(false, _stream, _fileDirectory));
  }
}

template<typename ValueType>
void ObjectDataLoader::readKeypoint(
            mtt::ValueKeypoint<ValueType, AnimationTrack::TimeType>& keypoint)
{
  uint32_t timeCount = _stream.readUint32();
  AnimationTrack::TimeType time(timeCount);
  keypoint.setTime(time);

  ValueType value;
  _stream >> value;
  keypoint.setValue(value);

  uint8_t interpolation = _stream.readInt8();
  keypoint.setInterpolation(mtt::InterpolationType(interpolation));
}

void ObjectDataLoader::visit(AnimationTrack& object)
{
  OEVisitor::visit(object);
  object.setEnabled(_stream.readBool());

  uint16_t keypointsNumber = _stream.readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<AnimationTrack::PositionKeypoint> keypoint(
                                      new AnimationTrack::PositionKeypoint());
    readKeypoint(*keypoint);
    object.addPositionKeypoint(std::move(keypoint));
  }

  keypointsNumber = _stream.readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<AnimationTrack::RotationKeypoint> keypoint(
                                      new AnimationTrack::RotationKeypoint());
    readKeypoint(*keypoint);
    object.addRotationKeypoint(std::move(keypoint));
  }

  keypointsNumber = _stream.readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<AnimationTrack::ScaleKeypoint> keypoint(
                                        new AnimationTrack::ScaleKeypoint());
    readKeypoint(*keypoint);
    object.addScaleKeypoint(std::move(keypoint));
  }

  object.setSkeletonId(_stream.readUID());
}

void ObjectDataLoader::visit(DisplayedObject& object)
{
  OEVisitor::visit(object);
  object.setVisible(_stream.readBool());
}

void ObjectDataLoader::visit(GeometryObject& object)
{
  OEVisitor::visit(object);
  object.setSkeletonId(_stream.readUID());
}

void ObjectDataLoader::visit(LODObject& object)
{
  OEVisitor::visit(object);
  object.setMinMppx(_stream.readFloat());
  object.setMaxMppx(_stream.readFloat());

  uint32_t childNumber = _stream.readUint32();
  for (; childNumber != 0; childNumber--)
  {
    object.addChild(loadObject<MeshObject>(false, _stream, _fileDirectory));
  }
}

QString ObjectDataLoader::loadFilename()
{
  QString relativePath;
  _stream >> relativePath;
  if(relativePath.isEmpty()) return QString();
  QFileInfo fileInfo(_fileDirectory.absoluteFilePath(relativePath));
  return fileInfo.canonicalFilePath();
}

void ObjectDataLoader::visit(MaterialObject& object)
{
  OEVisitor::visit(object);

  object.setAlbedo(_stream.readVec3());
  object.setAlbedoTexture(loadFilename());
  object.setUseAlphaFromAlbedoTexture(_stream.readBool());
  
  object.setRoughness(_stream.readFloat());
  object.setSpecularStrength(_stream.readFloat());
  object.setSpecularTexture(loadFilename());

  object.setMetallic(_stream.readFloat());
  
  object.setOpaqueFactor(_stream.readFloat());
  object.setOpaqueTexture(loadFilename());

  object.setReflectionFactor(_stream.readFloat());
  object.setReflectionTexture(loadFilename());

  object.setEmissionColor(_stream.readVec3());
  object.setEmissionFactor(_stream.readFloat());
  object.setEmissionTexture(loadFilename());

  object.setNormalTexture(loadFilename());
}

void ObjectDataLoader::readGeometry(mtt::CommonMeshGeometry& geometry)
{
  _stream >> geometry.positions;
  _stream >> geometry.normals;
  _stream >> geometry.tangents;
  _stream >> geometry.binormals;
  _stream >> geometry.texCoords;
  uint64_t skeletonRefsNumber = _stream.readUint64();
  geometry.skeletonRefs.resize(skeletonRefsNumber);
  for (mtt::CommonMeshGeometry::SkeletonRef& skeletonRef :
                                                        geometry.skeletonRefs)
  {
    _stream >> skeletonRef.bonesNumber;
    for ( uint16_t boneIndex = 0;
          boneIndex < skeletonRef.bonesNumber;
          boneIndex++)
    {
      _stream >> skeletonRef.bones[boneIndex].boneIndex;
      _stream >> skeletonRef.bones[boneIndex].weight;
    }
  }

  _stream >> geometry.triangleIndices;
  _stream >> geometry.lineIndices;
}

std::unique_ptr<BoneRefBatch> ObjectDataLoader::readBoneRefs()
{
  std::vector<std::unique_ptr<BoneRefObject>> refs;

  uint16_t boneRefsNumber = _stream.readUint16();
  for (;boneRefsNumber != 0; boneRefsNumber--)
  {
    QString name;
    _stream >> name;

    std::unique_ptr<BoneRefObject> refObject(new BoneRefObject(name, true));
    refObject->setBoneId(_stream.readUID());
    refObject->setBoneInverseMatrix(_stream.readMat4());
    refs.push_back(std::move(refObject));
  }

  return std::make_unique<BoneRefBatch>(
                                      std::move(refs),
                                      QCoreApplication::tr("Bone references"),
                                      false);
}

void ObjectDataLoader::visit(MeshObject& object)
{
  OEVisitor::visit(object);

  mtt::CommonMeshGeometry geometry;
  readGeometry(geometry);
  object.setGeometry(std::move(geometry));
  object.setBoneRefs(readBoneRefs());
  object.setMaterialId(_stream.readUID());
}

void ObjectDataLoader::visit(MovableObject& object)
{
  OEVisitor::visit(object);
  object.setPosition(_stream.readVec3());
}

void ObjectDataLoader::visit(RotatableObject& object)
{
  OEVisitor::visit(object);
  object.setRotation(_stream.readQuat());
}

void ObjectDataLoader::visit(ScalableObject& object)
{
  OEVisitor::visit(object);
  object.setScale(_stream.readVec3());
}

void ObjectDataLoader::visit(SkeletonObject& object)
{
  OEVisitor::visit(object);
  uint32_t childsNumber = _stream.readUint32();
  for (; childsNumber != 0; childsNumber--)
  {
    object.addChild(loadObject<SkeletonObject>(false, _stream, _fileDirectory));
  }
}

