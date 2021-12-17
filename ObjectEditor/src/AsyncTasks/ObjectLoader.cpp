#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

#include <AsyncTasks/ObjectLoader.h>

ObjectLoader::ObjectLoader( mtt::DataStream& stream,
                            const QDir& fileDirectory,
                            mtt::UID::ValueType mixUIDValue) :
  _stream(stream),
  _fileDirectory(fileDirectory),
  _mixUIDValue(mixUIDValue)
{
}

QString ObjectLoader::_loadFilename()
{
  QString relativePath;
  _stream >> relativePath;
  if(relativePath.isEmpty()) return QString();
  QFileInfo fileInfo(_fileDirectory.absoluteFilePath(relativePath));
  return fileInfo.canonicalFilePath();
}

void ObjectLoader::_readCubemapData(CubemapObject& object)
{
  CubemapObject::Textures textures;
  for (QString& filename : textures)
  {
    filename = _loadFilename();
  }
  object.setTextures(textures);
}

void ObjectLoader::visit(AmbientLightObject& object)
{
  OEVisitor::visit(object);
  object.setSaturationDistance(_stream.readFloat());
  _readCubemapData(object.ambientMap());
}

void ObjectLoader::visit(AnimationObject& object)
{
  OEVisitor::visit(object);
  uint32_t childNumber = _stream.readUint32();
  for (; childNumber != 0; childNumber--)
  {
    object.addChild(loadObject<AnimationTrack>( false,
                                                _stream,
                                                _fileDirectory,
                                                _mixUIDValue));
  }
}

template<typename ValueType>
void ObjectLoader::_readKeypoint(
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

mtt::UID ObjectLoader::_readUID()
{
  return _stream.readUID().mixedUID(_mixUIDValue);
}

void ObjectLoader::visit(AnimationTrack& object)
{
  OEVisitor::visit(object);
  object.setEnabled(_stream.readBool());

  uint16_t keypointsNumber = _stream.readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<AnimationTrack::PositionKeypoint> keypoint(
                                      new AnimationTrack::PositionKeypoint());
    _readKeypoint(*keypoint);
    object.addPositionKeypoint(std::move(keypoint));
  }

  keypointsNumber = _stream.readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<AnimationTrack::RotationKeypoint> keypoint(
                                      new AnimationTrack::RotationKeypoint());
    _readKeypoint(*keypoint);
    object.addRotationKeypoint(std::move(keypoint));
  }

  keypointsNumber = _stream.readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<AnimationTrack::ScaleKeypoint> keypoint(
                                        new AnimationTrack::ScaleKeypoint());
    _readKeypoint(*keypoint);
    object.addScaleKeypoint(std::move(keypoint));
  }

  object.setSkeletonId(_readUID());
}

void ObjectLoader::visit(BackgroundObject& object)
{
  OEVisitor::visit(object);
  object.setLightEnabled(_stream.readBool());
  object.setLuminance(_stream.readFloat());
  object.setColor(_stream.readVec3());
  object.setDissolutionStartDistance(_stream.readFloat());
  object.setDissolutionLength(_stream.readFloat());
  _readCubemapData(object.cubemap());
}

void ObjectLoader::visit(DirectLightObject& object)
{
  OEVisitor::visit(object);
  object.setRadius(_stream.readFloat());
  object.setShadowsEnabled(_stream.readBool());
  object.setShadowmapSize(_stream.readUint16());
  object.setCascadeSize(_stream.readUint8());
  object.setBlurSize(_stream.readFloat());
}

void ObjectLoader::visit(DisplayedObject& object)
{
  OEVisitor::visit(object);
  object.setVisible(_stream.readBool());
}

void ObjectLoader::visit(EnvironmentModel& object)
{
  OEVisitor::visit(object);
  object.setFilename(_loadFilename());
}

void ObjectLoader::visit(GeometryObject& object)
{
  OEVisitor::visit(object);
  object.setSkeletonId(_readUID());
}

void ObjectLoader::visit(LightObject& object)
{
  OEVisitor::visit(object);
  object.setEnabled(_stream.readBool());
  object.setDistance(_stream.readFloat());
  object.setColor(_stream.readVec3());
  object.setBaseIlluminance(_stream.readFloat());
}

void ObjectLoader::visit(LODObject& object)
{
  OEVisitor::visit(object);
  object.setMinMppx(_stream.readFloat());
  object.setMaxMppx(_stream.readFloat());

  uint32_t childNumber = _stream.readUint32();
  for (; childNumber != 0; childNumber--)
  {
    object.addChild(loadObject<MeshObject>( false,
                                            _stream,
                                            _fileDirectory,
                                            _mixUIDValue));
  }
}

void ObjectLoader::visit(MaterialObject& object)
{
  OEVisitor::visit(object);

  object.setAlbedo(_stream.readVec3());
  object.setAlbedoTexture(_loadFilename());
  object.setUseAlphaFromAlbedoTexture(_stream.readBool());
  
  object.setRoughness(_stream.readFloat());
  object.setSpecularStrength(_stream.readFloat());
  object.setSpecularTexture(_loadFilename());

  object.setMetallic(_stream.readFloat());
  
  object.setOpaqueFactor(_stream.readFloat());
  object.setOpaqueTexture(_loadFilename());

  object.setReflectionFactor(_stream.readFloat());
  object.setReflectionTexture(_loadFilename());

  object.setEmissionColor(_stream.readVec3());
  object.setEmissionFactor(_stream.readFloat());
  object.setEmissionTexture(_loadFilename());

  object.setNormalTexture(_loadFilename());
}

void ObjectLoader::_readGeometry(mtt::CommonMeshGeometry& geometry)
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

std::unique_ptr<BoneRefBatch> ObjectLoader::readBoneRefs()
{
  std::vector<std::unique_ptr<BoneRefObject>> refs;

  uint16_t boneRefsNumber = _stream.readUint16();
  for (;boneRefsNumber != 0; boneRefsNumber--)
  {
    QString name;
    _stream >> name;

    std::unique_ptr<BoneRefObject> refObject(new BoneRefObject(name, true));
    refObject->setBoneId(_readUID());
    refObject->setBoneInverseMatrix(_stream.readMat4());
    refs.push_back(std::move(refObject));
  }

  return std::make_unique<BoneRefBatch>(
                                      std::move(refs),
                                      QCoreApplication::tr("Bone references"),
                                      false);
}

void ObjectLoader::visit(MeshObject& object)
{
  OEVisitor::visit(object);

  mtt::CommonMeshGeometry geometry;
  _readGeometry(geometry);
  object.setGeometry(std::move(geometry));
  object.setBoneRefs(readBoneRefs());
  object.setMaterialId(_readUID());
}

void ObjectLoader::visit(MovableObject& object)
{
  OEVisitor::visit(object);
  object.setPosition(_stream.readVec3());
}

void ObjectLoader::visit(RotatableObject& object)
{
  OEVisitor::visit(object);
  object.setRotation(_stream.readQuat());
}

void ObjectLoader::visit(ScalableObject& object)
{
  OEVisitor::visit(object);
  object.setScale(_stream.readVec3());
}

void ObjectLoader::visit(SkeletonObject& object)
{
  OEVisitor::visit(object);
  uint32_t childsNumber = _stream.readUint32();
  for (; childsNumber != 0; childsNumber--)
  {
    object.addChild(loadObject<SkeletonObject>( false,
                                                _stream,
                                                _fileDirectory,
                                                _mixUIDValue));
  }
}

void ObjectLoader::_loadObjectData( mtt::Object& object,
                                    mtt::DataStream& stream,
                                    const QDir& fileDirectory,
                                    mtt::UID::ValueType mixUIDValue)
{
  ObjectLoader loader(stream, fileDirectory, mixUIDValue);
  loader.process(object);
}
