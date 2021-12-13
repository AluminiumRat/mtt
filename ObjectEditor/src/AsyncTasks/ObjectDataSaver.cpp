#include <mtt/Application/DataStream.h>

#include <AsyncTasks/ObjectBuilder.h>
#include <AsyncTasks/ObjectDataSaver.h>

ObjectDataSaver::ObjectDataSaver( mtt::DataStream& stream,
                                  const QDir& fileDirectory) :
  _stream(stream),
  _fileDirectory(fileDirectory)
{
}

void ObjectDataSaver::saveObject( const mtt::Object& object,
                                  mtt::DataStream& stream,
                                  const QDir& fileDirectory)
{
  ObjectBuilder::ObjectType type = ObjectBuilder::getObjectType(object);
  stream << type;

  ObjectDataSaver dataSaver(stream, fileDirectory);
  dataSaver.process(object);
}

void ObjectDataSaver::visit(const AnimationObject& object)
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
void ObjectDataSaver::writeKeypoint(
              mtt::ValueKeypoint<ValueType, AnimationTrack::TimeType> keypoint)
{
  _stream << (uint32_t)keypoint.time().count();
  _stream << keypoint.value();
  _stream << (uint8_t) keypoint.interpolation();
}

void ObjectDataSaver::visit(const AnimationTrack& object)
{
  OEVisitor::visit(object);
  _stream << object.enabled();

  _stream <<(uint16_t)object.positionKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.positionKeypointNumber();
        keypointIndex++)
  {
    writeKeypoint(object.positionKeypoint(keypointIndex));
  }

  _stream <<(uint16_t)object.rotationKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.rotationKeypointNumber();
        keypointIndex++)
  {
    writeKeypoint(object.rotationKeypoint(keypointIndex));
  }

  _stream <<(uint16_t)object.scaleKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.scaleKeypointNumber();
        keypointIndex++)
  {
    writeKeypoint(object.scaleKeypoint(keypointIndex));
  }
  
  _stream << object.skeletonRef().referencedId();
}

void ObjectDataSaver::visit(const DisplayedObject& object)
{
  OEVisitor::visit(object);
  _stream << object.visible();
}

void ObjectDataSaver::visit(const GeometryObject& object)
{
  OEVisitor::visit(object);
  _stream << object.skeletonRef().referencedId();
}

void ObjectDataSaver::visit(const LODObject& object)
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

void ObjectDataSaver::writeFilename(const QString& filename)
{
  _stream << _fileDirectory.relativeFilePath(filename);
}

void ObjectDataSaver::visit(const MaterialObject& object)
{
  OEVisitor::visit(object);
    
  _stream << object.albedo();
  writeFilename(object.albedoTexture());
  _stream << object.useAlphaFromAlbedoTexture();
    
  _stream << object.roughness();
  _stream << object.specularStrength();
  writeFilename(object.specularTexture());

  _stream << object.metallic();
    
  _stream << object.opaqueFactor();
  writeFilename(object.opaqueTexture());

  _stream << object.reflectionFactor();
  writeFilename(object.reflectionTexture());

  _stream << object.emissionColor();
  _stream << object.emissionFactor();
  writeFilename(object.emissionTexture());

  writeFilename(object.normalTexture());
}

void ObjectDataSaver::saveGeometry(const mtt::CommonMeshGeometry& geometry)
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

void ObjectDataSaver::saveBoneRefs(const BoneRefBatch& refs)
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

void ObjectDataSaver::visit(const MeshObject& object)
{
  OEVisitor::visit(object);

  const mtt::CommonMeshGeometry& geometry = object.geometry();
  saveGeometry(geometry);
  saveBoneRefs(object.boneRefs());
  _stream << object.materialRef().referencedId();
}

void ObjectDataSaver::visit(const MovableObject& object)
{
  OEVisitor::visit(object);
  _stream << object.position();
}

void ObjectDataSaver::visit(const mtt::Object& object)
{
  _stream << object.id();
  _stream << object.name();
}

void ObjectDataSaver::visit(const RotatableObject& object)
{
  OEVisitor::visit(object);
  _stream << object.rotation();
}

void ObjectDataSaver::visit(const ScalableObject& object)
{
  OEVisitor::visit(object);
  _stream << object.scale();
}

void ObjectDataSaver::visit(const SkeletonObject& object)
{
  OEVisitor::visit(object);
  uint32_t childNumber = object.childsNumber();
  _stream << childNumber;
  for (uint32_t childIndex = 0; childIndex < childNumber; childIndex++)
  {
    saveObject(object.child(childIndex), _stream, _fileDirectory);
  }
}
