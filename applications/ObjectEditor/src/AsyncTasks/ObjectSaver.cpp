#include <mtt/Application/DataStream.h>

#include <AsyncTasks/ObjectBuilder.h>
#include <AsyncTasks/ObjectSaver.h>

ObjectSaver::ObjectSaver( mtt::DataStream& stream,
                          const QDir& fileDirectory) :
  _stream(stream),
  _fileDirectory(fileDirectory)
{
}

void ObjectSaver::saveObject( const mtt::Object& object,
                              mtt::DataStream& stream,
                              const QDir& fileDirectory)
{
  ObjectBuilder::ObjectType type = ObjectBuilder::getObjectType(object);
  stream << type;

  ObjectSaver dataSaver(stream, fileDirectory);
  dataSaver.process(object);
}

void ObjectSaver::_writeFilename(const QString& filename)
{
  _stream << _fileDirectory.relativeFilePath(filename);
}

void ObjectSaver::_writeCubemapData(const CubemapObject& object)
{
  CubemapObject::Textures textures = object.textures();
  for (const QString& filename : textures)
  {
    _writeFilename(filename);
  }
}

void ObjectSaver::visit(const AmbientLightObject& object)
{
  OEVisitor::visit(object);
  _stream << object.saturationDistance();
  _writeCubemapData(object.ambientMap());
}

void ObjectSaver::visit(const AnimationObject& object)
{
  OEVisitor::visit(object);
  uint32_t childNumber = object.childsNumber();
  _stream << childNumber;
  for (uint32_t childIndex = 0; childIndex < childNumber; childIndex++)
  {
    saveObject(object.child(childIndex), _stream, _fileDirectory);
  }
}

template<typename ValueType>
void ObjectSaver::_writeKeypoint(
              mtt::ValueKeypoint<ValueType, AnimationTrack::TimeType> keypoint)
{
  _stream << (uint32_t)keypoint.time().count();
  _stream << keypoint.value();
  _stream << (uint8_t) keypoint.interpolation();
}

void ObjectSaver::visit(const AnimationTrack& object)
{
  OEVisitor::visit(object);
  _stream << object.enabled();

  _stream <<(uint16_t)object.positionKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.positionKeypointNumber();
        keypointIndex++)
  {
    _writeKeypoint(object.positionKeypoint(keypointIndex));
  }

  _stream <<(uint16_t)object.rotationKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.rotationKeypointNumber();
        keypointIndex++)
  {
    _writeKeypoint(object.rotationKeypoint(keypointIndex));
  }

  _stream <<(uint16_t)object.scaleKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.scaleKeypointNumber();
        keypointIndex++)
  {
    _writeKeypoint(object.scaleKeypoint(keypointIndex));
  }
  
  _stream << object.skeletonRef().referencedId();
}

void ObjectSaver::visit(const BackgroundObject& object)
{
  OEVisitor::visit(object);
  _stream << object.lightEnabled();
  _stream << object.luminance();
  _stream << object.color();
  _stream << object.dissolutionStartDistance();
  _stream << object.dissolutionLength();
  _writeCubemapData(object.cubemap());
}

void ObjectSaver::visit(const DirectLightObject& object)
{
  OEVisitor::visit(object);
  _stream << object.radius();
  _stream << object.shadowsEnabled();
  _stream << (uint16_t)object.shadowmapSize();
  _stream << (uint8_t)object.cascadeSize();
  _stream << object.blurSize();
}

void ObjectSaver::visit(const DisplayedObject& object)
{
  OEVisitor::visit(object);
  _stream << object.visible();
}

void ObjectSaver::visit(const EnvironmentModel& object)
{
  OEVisitor::visit(object);
  _writeFilename(object.filename());
}

void ObjectSaver::visit(const GeometryObject& object)
{
  OEVisitor::visit(object);
  _stream << object.skeletonRef().referencedId();
}

void ObjectSaver::visit(const LightObject& object)
{
  OEVisitor::visit(object);
  _stream << object.enabled();
  _stream << object.distance();
  _stream << object.color();
  _stream << object.baseIlluminance();
}

void ObjectSaver::visit(const LODObject& object)
{
  OEVisitor::visit(object);
  _stream << object.minMppx();
  _stream << object.maxMppx();

  uint32_t childNumber = object.childsNumber();
  _stream << childNumber;
  for (uint32_t childIndex = 0; childIndex < childNumber; childIndex++)
  {
    saveObject( object.child(childIndex), _stream, _fileDirectory);
  }
}

void ObjectSaver::visit(const MaterialObject& object)
{
  OEVisitor::visit(object);
    
  _stream << object.albedo();
  _writeFilename(object.albedoTexture());
  _stream << object.useAlphaFromAlbedoTexture();
    
  _stream << object.roughness();
  _stream << object.specularStrength();
  _writeFilename(object.specularTexture());

  _stream << object.metallic();
    
  _stream << object.opaqueFactor();
  _writeFilename(object.opaqueTexture());

  _stream << object.reflectionFactor();
  _writeFilename(object.reflectionTexture());

  _stream << object.emissionColor();
  _stream << object.emissionFactor();
  _writeFilename(object.emissionTexture());

  _writeFilename(object.normalTexture());
}

void ObjectSaver::_saveGeometry(const mtt::CommonMeshGeometry& geometry)
{
  _stream << geometry.positions;
  _stream << geometry.normals;
  _stream << geometry.tangents;
  _stream << geometry.binormals;
  _stream << geometry.texCoords;
  uint64_t skeletonRefsNumber = geometry.skeletonRefs.size();
  _stream << skeletonRefsNumber;
  for (const mtt::CommonMeshGeometry::SkeletonRef& skeletonRef :
                                                        geometry.skeletonRefs)
  {
    _stream << skeletonRef.bonesNumber;
    for ( uint16_t boneIndex = 0;
          boneIndex < skeletonRef.bonesNumber;
          boneIndex++)
    {
      _stream << skeletonRef.bones[boneIndex].boneIndex;
      _stream << skeletonRef.bones[boneIndex].weight;
    }
  }

  _stream << geometry.triangleIndices;
  _stream << geometry.lineIndices;
}

void ObjectSaver::_saveBoneRefs(const BoneRefBatch& refs)
{
  _stream << (uint16_t) refs.boneRefsNumber();
  for (size_t refIndex = 0; refIndex < refs.boneRefsNumber(); refIndex++)
  {
    const BoneRefObject& boneRef = refs.boneRef(refIndex);
    _stream << boneRef.name();
    _stream << boneRef.boneRef().referencedId();
    _stream << boneRef.boneInverseMatrix();
  }
}

void ObjectSaver::visit(const MeshObject& object)
{
  OEVisitor::visit(object);

  const mtt::CommonMeshGeometry& geometry = object.geometry();
  _saveGeometry(geometry);
  _saveBoneRefs(object.boneRefs());
  _stream << object.materialRef().referencedId();
}

void ObjectSaver::visit(const MovableObject& object)
{
  OEVisitor::visit(object);
  _stream << object.position();
}

void ObjectSaver::visit(const mtt::Object& object)
{
  _stream << object.id();
  _stream << object.name();
}

void ObjectSaver::visit(const RotatableObject& object)
{
  OEVisitor::visit(object);
  _stream << object.rotation();
}

void ObjectSaver::visit(const ScalableObject& object)
{
  OEVisitor::visit(object);
  _stream << object.scale();
}

void ObjectSaver::visit(const SkeletonObject& object)
{
  OEVisitor::visit(object);
  uint32_t childNumber = object.childsNumber();
  _stream << childNumber;
  for (uint32_t childIndex = 0; childIndex < childNumber; childIndex++)
  {
    saveObject(object.child(childIndex), _stream, _fileDirectory);
  }
}
