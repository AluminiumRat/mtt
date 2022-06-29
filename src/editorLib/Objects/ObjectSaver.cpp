#include <stdexcept>

#include <mtt/editorLib/Objects/ObjectSaver.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

ObjectSaver::ObjectSaver() :
  _stream(nullptr),
  _objectFactory(nullptr)
{
}

void ObjectSaver::saveObject( const mtt::Object& object,
                              mtt::DataStream& stream,
                              const QDir& fileDirectory,
                              const ObjectFactory& objectFactory)
{
  uint16_t typeIndex = objectFactory.getTypeIndex(object);
  if (typeIndex == ObjectFactory::notIndex)
  {
    throw std::runtime_error("Unsupported object type");
  }
  stream << typeIndex;
  stream << object.id();
  saveObjectData(object, stream, fileDirectory, objectFactory);
}

void ObjectSaver::saveObjectData( const Object& object,
                                  DataStream& stream,
                                  const QDir& fileDirectory,
                                  const ObjectFactory& objectFactory)
{
  ScopedSetter<DataStream*> setStream(_stream, &stream);
  ScopedSetter<QDir> setFileDir(_fileDirectory, fileDirectory);
  ScopedSetter<const ObjectFactory*> setObjectFactory(_objectFactory,
                                                      &objectFactory);
  process(object);
}

void ObjectSaver::writeFilename(const QString& filename)
{
  *_stream << _fileDirectory.relativeFilePath(filename);
}

void ObjectSaver::writeCubemapData(const mtt::CubemapObject& object)
{
  mtt::CubemapObject::Textures textures = object.textures();
  for (const QString& filename : textures)
  {
    writeFilename(filename);
  }
}

void ObjectSaver::writeChilds(const Object& parent)
{
  uint32_t objectsNumber = uint32_t(parent.subobjectNumber());
  stream() << objectsNumber;
  for(uint32_t objectIndex = 0;
      objectIndex < objectsNumber;
      objectIndex++)
  {
    saveObject( parent.subobject(objectIndex),
                stream(),
                fileDirectory(),
                objectFactory());
  }
}

void ObjectSaver::visitConstAmbientLightObject(
                                          const mtt::AmbientLightObject& object)
{
  CEVisitor::visitConstAmbientLightObject(object);
  *_stream << object.saturationDistance();
  writeCubemapData(object.ambientMap());
}

void ObjectSaver::visitConstAnimationGroup(const AnimationGroup& object)
{
  CEVisitor::visitConstAnimationGroup(object);
  writeChilds(object);
}

void ObjectSaver::visitConstAnimationObject(const mtt::AnimationObject& object)
{
  CEVisitor::visitConstAnimationObject(object);
  uint32_t childNumber = uint32_t(object.childsNumber());
  *_stream << childNumber;
  for (uint32_t childIndex = 0; childIndex < childNumber; childIndex++)
  {
    saveObject( object.child(childIndex),
                *_stream,
                _fileDirectory,
                *_objectFactory);
  }
}
void ObjectSaver::visitConstAnimationTrack(const mtt::AnimationTrack& object)
{
  CEVisitor::visitConstAnimationTrack(object);
  *_stream << object.enabled();
}

void ObjectSaver::visitConstBackgroundObject(
                                            const mtt::BackgroundObject& object)
{
  CEVisitor::visitConstBackgroundObject(object);
  *_stream << object.lightEnabled();
  *_stream << object.luminance();
  *_stream << object.color();
  *_stream << object.dissolutionStartDistance();
  *_stream << object.dissolutionLength();
  writeCubemapData(object.cubemap());
}

void ObjectSaver::visitConstCubemapObject(const CubemapObject& object)
{
  CEVisitor::visitConstCubemapObject(object);
  writeCubemapData(object);
}

void ObjectSaver::visitConstDirectLightObject(
                                          const mtt::DirectLightObject& object)
{
  CEVisitor::visitConstDirectLightObject(object);
  *_stream << (uint8_t)object.cascadeSize();
  *_stream << object.shadowDistance();
}

void ObjectSaver::visitConstDisplayedObject(const mtt::DisplayedObject& object)
{
  CEVisitor::visitConstDisplayedObject(object);
  *_stream << object.visible();
}

void ObjectSaver::visitConstEnvironmentGroup(const EnvironmentGroup& object)
{
  CEVisitor::visitConstEnvironmentGroup(object);
  writeChilds(object);
}

void ObjectSaver::visitConstEnvironmentModel(
                                            const mtt::EnvironmentModel& object)
{
  CEVisitor::visitConstEnvironmentModel(object);
  writeFilename(object.filename());
}

void ObjectSaver::visitConstEnvironmentRootObject(
                                            const EnvironmentRootObject& object)
{
  CEVisitor::visitConstEnvironmentRootObject(object);

  saveObjectData( object.background(),
                  stream(),
                  fileDirectory(),
                  objectFactory());

  saveObjectData( object.objectsGroup(),
                  stream(),
                  fileDirectory(),
                  objectFactory());
}

void ObjectSaver::visitConstLightObject(const mtt::LightObject& object)
{
  CEVisitor::visitConstLightObject(object);
  *_stream << object.enabled();
  *_stream << object.distance();
  *_stream << object.color();
  *_stream << object.baseIlluminance();
}

void ObjectSaver::visitConstLightWithShadowsObject(
                                          const LightWithShadowsObject& object)
{
  CEVisitor::visitConstLightWithShadowsObject(object);
  *_stream << object.shadowsEnabled();
  *_stream << (uint16_t)object.shadowmapSize();
  *_stream << object.shadowBlur();
}

void ObjectSaver::visitConstMovableObject(const mtt::MovableObject& object)
{
  CEVisitor::visitConstMovableObject(object);
  *_stream << object.position();
}

void ObjectSaver::visitConstPointLightObject(const PointLightObject& object)
{
  CEVisitor::visitConstPointLightObject(object);
  writeCubemapData(object.filterCubemap());
}

void ObjectSaver::visitConstPositionAnimator(const PositionAnimator& object)
{
  CEVisitor::visitConstPositionAnimator(object);

  *_stream <<(uint16_t)object.positionKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.positionKeypointNumber();
        keypointIndex++)
  {
    writeKeypoint(object.positionKeypoint(keypointIndex));
  }

  *_stream <<(uint16_t)object.rotationKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.rotationKeypointNumber();
        keypointIndex++)
  {
    writeKeypoint(object.rotationKeypoint(keypointIndex));
  }

  *_stream <<(uint16_t)object.scaleKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.scaleKeypointNumber();
        keypointIndex++)
  {
    writeKeypoint(object.scaleKeypoint(keypointIndex));
  }
  
  *_stream << object.targetRef().referencedId();
}

void ObjectSaver::visitConstObject(const mtt::Object& object)
{
  *_stream << object.name();
}

void ObjectSaver::visitConstRotatableObject(const mtt::RotatableObject& object)
{
  CEVisitor::visitConstRotatableObject(object);
  *_stream << object.rotation();
}

void ObjectSaver::visitConstScalableObject(const mtt::ScalableObject& object)
{
  CEVisitor::visitConstScalableObject(object);
  *_stream << object.scale();
}

void ObjectSaver::visitConstSkeletonGroup(const SkeletonGroup& object)
{
  CEVisitor::visitConstSkeletonGroup(object);
  writeChilds(object);
}

void ObjectSaver::visitConstSkeletonObject(const mtt::SkeletonObject& object)
{
  CEVisitor::visitConstSkeletonObject(object);
  uint32_t childNumber = uint32_t(object.childsNumber());
  *_stream << childNumber;
  for (uint32_t childIndex = 0; childIndex < childNumber; childIndex++)
  {
    saveObject( object.child(childIndex),
                *_stream,
                _fileDirectory,
                *_objectFactory);
  }
}

void ObjectSaver::visitConstSpotLightObject(const SpotLightObject& object)
{
  CEVisitor::visitConstSpotLightObject(object);
  *_stream << object.angle();
  writeFilename(object.filterImage());
}
