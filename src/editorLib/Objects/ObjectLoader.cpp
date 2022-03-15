#include <mtt/editorLib/Objects/ObjectLoader.h>

using namespace mtt;

ObjectLoader::ObjectLoader() :
  _stream(nullptr),
  _objectFactory(nullptr)
{
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

  uint16_t keypointsNumber = _stream->readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<AnimationTrack::PositionKeypoint> keypoint(
                                  new AnimationTrack::PositionKeypoint());
    readKeypoint(*keypoint);
    object.addPositionKeypoint(std::move(keypoint));
  }

  keypointsNumber = _stream->readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<AnimationTrack::RotationKeypoint> keypoint(
                                  new AnimationTrack::RotationKeypoint());
    readKeypoint(*keypoint);
    object.addRotationKeypoint(std::move(keypoint));
  }

  keypointsNumber = _stream->readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<AnimationTrack::ScaleKeypoint> keypoint(
                                      new AnimationTrack::ScaleKeypoint());
    readKeypoint(*keypoint);
    object.addScaleKeypoint(std::move(keypoint));
  }

  object.skeletonRef().setReferencedId(readUID());
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
  object.setRadius(_stream->readFloat());
  object.setShadowsEnabled(_stream->readBool());
  object.setShadowmapSize(_stream->readUint16());
  object.setCascadeSize(_stream->readUint8());
  object.setBlurSize(_stream->readFloat());
}

void ObjectLoader::visitDisplayedObject(DisplayedObject& object)
{
  CEVisitor::visitDisplayedObject(object);
  object.setVisible(_stream->readBool());
}

void ObjectLoader::visitEnvironmentModel(EnvironmentModel& object)
{
  CEVisitor::visitEnvironmentModel(object);
  object.setFilename(readFilename());
}

void ObjectLoader::visitLightObject(LightObject& object)
{
  CEVisitor::visitLightObject(object);
  object.setEnabled(_stream->readBool());
  object.setDistance(_stream->readFloat());
  object.setColor(_stream->readVec3());
  object.setBaseIlluminance(_stream->readFloat());
}

void ObjectLoader::visitMovableObject(MovableObject& object)
{
  CEVisitor::visitMovableObject(object);
  object.setPosition(_stream->readVec3());
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
