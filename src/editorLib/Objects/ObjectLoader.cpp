#include <mtt/editorLib/Objects/ObjectLoader.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

ObjectLoader::ObjectLoader() :
  _stream(nullptr),
  _objectFactory(nullptr)
{
}

void ObjectLoader::loadEmbeddedObject(Object& object,
                                      DataStream& stream,
                                      const QDir& fileDirectory,
                                      UID::ValueType mixUIDValue,
                                      const ObjectFactory& objectFactory)
{
  QString name;
  stream >> name;
  object.tryRename(name);
  _readObjectData(object, stream, fileDirectory, mixUIDValue, objectFactory);
}

void ObjectLoader::_readObjectData( Object& object,
                                    DataStream& stream,
                                    const QDir& fileDirectory,
                                    UID::ValueType mixUIDValue,
                                    const ObjectFactory& objectFactory)
{
  ScopedSetter<DataStream*> setStream(_stream, &stream);
  ScopedSetter<QDir> setFileDir(_fileDirectory, fileDirectory);
  ScopedSetter<UID::ValueType> setUidMixValue(_mixUIDValue, mixUIDValue);
  ScopedSetter<const ObjectFactory*> setObjectFactory(_objectFactory,
                                                      &objectFactory);

  process(object);
}

QString ObjectLoader::readFilename()
{
  QString relativePath;
  *_stream >> relativePath;
  if(relativePath.isEmpty()) return QString();
  QFileInfo fileInfo(_fileDirectory.absoluteFilePath(relativePath));
  return fileInfo.canonicalFilePath();
}

void ObjectLoader::visitAmbientLightObject(AmbientLightObject& object)
{
  CEVisitor::visitAmbientLightObject(object);
  object.setSaturationDistance(_stream->readFloat());
  readCubemapData(object.ambientMap());
}

void ObjectLoader::visitAnimationGroup(AnimationGroup& object)
{
  CEVisitor::visitAnimationGroup(object);
  readChilds<AnimationGroup, AnimationObject>(object, true);
}

void ObjectLoader::visitAnimationObject(AnimationObject& object)
{
  CEVisitor::visitAnimationObject(object);
  uint32_t childNumber = _stream->readUint32();
  for (; childNumber != 0; childNumber--)
  {
    object.addChild(loadObject<AnimationTrack>( true,
                                                *_stream,
                                                _fileDirectory,
                                                _mixUIDValue,
                                                *_objectFactory));
  }
}

UID ObjectLoader::readUID()
{
  return _stream->readUID().mixedUID(_mixUIDValue);
}

void ObjectLoader::visitAnimationTrack(AnimationTrack& object)
{
  CEVisitor::visitAnimationTrack(object);
  object.setEnabled(_stream->readBool());
}

void ObjectLoader::visitBackgroundObject(BackgroundObject& object)
{
  CEVisitor::visitBackgroundObject(object);
  object.setLightEnabled(_stream->readBool());
  object.setLuminance(_stream->readFloat());
  object.setColor(_stream->readVec3());
  object.setDissolutionStartDistance(_stream->readFloat());
  object.setDissolutionLength(_stream->readFloat());
  readCubemapData(object.cubemap());
}

void ObjectLoader::readCubemapData(CubemapObject& object)
{
  CubemapObject::Textures textures;
  for (QString& filename : textures)
  {
    filename = readFilename();
  }
  object.setTextures(textures);
}

void ObjectLoader::visitCubemapObject(CubemapObject& object)
{
  CEVisitor::visitCubemapObject(object);
  readCubemapData(object);
}

void ObjectLoader::visitDirectLightObject(DirectLightObject& object)
{
  CEVisitor::visitDirectLightObject(object);
  object.setCascadeSize(_stream->readUint8());
  object.setRadius(_stream->readFloat());
}

void ObjectLoader::visitDisplayedObject(DisplayedObject& object)
{
  CEVisitor::visitDisplayedObject(object);
  object.setVisible(_stream->readBool());
}

void ObjectLoader::visitEnvironmentGroup(EnvironmentGroup& object)
{
  CEVisitor::visitEnvironmentGroup(object);
  readChilds<EnvironmentGroup, EnvironmentObject>(object, true);
}

void ObjectLoader::visitEnvironmentModel(EnvironmentModel& object)
{
  CEVisitor::visitEnvironmentModel(object);
  object.setFilename(readFilename());
}

void ObjectLoader::visitEnvironmentRootObject(EnvironmentRootObject& object)
{
  CEVisitor::visitEnvironmentRootObject(object);

  loadEmbeddedObject( object.background(),
                      stream(),
                      fileDirectory(),
                      mixUIDValue(),
                      objectFactory());

  loadEmbeddedObject( object.objectsGroup(),
                      stream(),
                      fileDirectory(),
                      mixUIDValue(),
                      objectFactory());
}

void ObjectLoader::visitLightObject(LightObject& object)
{
  CEVisitor::visitLightObject(object);
  object.setEnabled(_stream->readBool());
  object.setDistance(_stream->readFloat());
  object.setColor(_stream->readVec3());
  object.setBaseIlluminance(_stream->readFloat());
}

void ObjectLoader::visitLightWithShadowsObject(LightWithShadowsObject& object)
{
  CEVisitor::visitLightWithShadowsObject(object);
  object.setShadowsEnabled(_stream->readBool());
  object.setShadowmapSize(_stream->readUint16());
  object.setShadowBlur(_stream->readFloat());
}

void ObjectLoader::visitMovableObject(MovableObject& object)
{
  CEVisitor::visitMovableObject(object);
  object.setPosition(_stream->readVec3());
}

void ObjectLoader::visitPointLightObject(PointLightObject& object)
{
  CEVisitor::visitPointLightObject(object);
  readCubemapData(object.filterCubemap());
}

void ObjectLoader::visitPositionAnimator(PositionAnimator& object)
{
  CEVisitor::visitPositionAnimator(object);

  uint16_t keypointsNumber = _stream->readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<PositionAnimator::PositionKeypoint> keypoint(
                                      new PositionAnimator::PositionKeypoint());
    readKeypoint(*keypoint);
    object.addPositionKeypoint(std::move(keypoint));
  }

  keypointsNumber = _stream->readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<PositionAnimator::RotationKeypoint> keypoint(
                                      new PositionAnimator::RotationKeypoint());
    readKeypoint(*keypoint);
    object.addRotationKeypoint(std::move(keypoint));
  }

  keypointsNumber = _stream->readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<PositionAnimator::ScaleKeypoint> keypoint(
                                        new PositionAnimator::ScaleKeypoint());
    readKeypoint(*keypoint);
    object.addScaleKeypoint(std::move(keypoint));
  }

  object.targetRef().setReferencedId(readUID());
}

void ObjectLoader::visitRotatableObject(RotatableObject& object)
{
  CEVisitor::visitRotatableObject(object);
  object.setRotation(_stream->readQuat());
}

void ObjectLoader::visitScalableObject(ScalableObject& object)
{
  CEVisitor::visitScalableObject(object);
  object.setScale(_stream->readVec3());
}

void ObjectLoader::visitSkeletonGroup(SkeletonGroup& object)
{
  CEVisitor::visitSkeletonGroup(object);
  readChilds<SkeletonGroup, SkeletonObject>(object, true);
}

void ObjectLoader::visitSkeletonObject(SkeletonObject& object)
{
  CEVisitor::visitSkeletonObject(object);
  uint32_t childsNumber = _stream->readUint32();
  for (; childsNumber != 0; childsNumber--)
  {
    object.addChild(loadObject<SkeletonObject>( true,
                                                *_stream,
                                                _fileDirectory,
                                                _mixUIDValue,
                                                *_objectFactory));
  }
}

void ObjectLoader::visitSpotLightObject(SpotLightObject& object)
{
  CEVisitor::visitSpotLightObject(object);
  object.setAngle(_stream->readFloat());
  object.setFilterImage(readFilename());
}
