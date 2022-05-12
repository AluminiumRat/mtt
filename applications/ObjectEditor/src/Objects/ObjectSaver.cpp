#include <Objects/ObjectSaver.h>

void ObjectSaver::visitConstGeometryGroup(const GeometryGroup& object)
{
  OEVisitorT::visitConstGeometryGroup(object);
  writeChilds(object);
}

void ObjectSaver::visitConstGeometryObject(const GeometryObject& object)
{
  OEVisitorT::visitConstGeometryObject(object);
  stream() << object.skeletonRef().referencedId();
}

void ObjectSaver::visitConstLODObject(const LODObject& object)
{
  OEVisitorT::visitConstLODObject(object);
  stream() << object.minMppx();
  stream() << object.maxMppx();

  uint32_t childNumber = object.childsNumber();
  stream() << childNumber;
  for (uint32_t childIndex = 0; childIndex < childNumber; childIndex++)
  {
    saveObject( object.child(childIndex),
                stream(),
                fileDirectory(),
                objectFactory());
  }
}

void ObjectSaver::visitConstMaterialsGroup(const MaterialsGroup& object)
{
  OEVisitorT::visitConstMaterialsGroup(object);
  writeChilds(object);
}

void ObjectSaver::visitConstMaterialObject(const MaterialObject& object)
{
  OEVisitorT::visitConstMaterialObject(object);

  stream() << object.albedo();
  writeFilename(object.albedoTexture());
  stream() << object.useAlphaFromAlbedoTexture();

  stream() << object.roughness();
  stream() << object.specularStrength();
  writeFilename(object.specularTexture());

  stream() << object.metallic();

  stream() << object.opaqueFactor();
  writeFilename(object.opaqueTexture());

  stream() << object.reflectionFactor();
  writeFilename(object.reflectionTexture());

  stream() << object.emissionColor();
  stream() << object.emissionFactor();
  writeFilename(object.emissionTexture());

  writeFilename(object.normalTexture());
}

void ObjectSaver::visitConstMeshObject(const MeshObject& object)
{
  OEVisitorT::visitConstMeshObject(object);

  const mtt::CommonMeshGeometry& geometry = object.geometry();
  saveGeometry(geometry);
  saveBoneRefs(object.boneRefs());
  stream() << object.materialRef().referencedId();
}

void ObjectSaver::saveGeometry(const mtt::CommonMeshGeometry& geometry)
{
  stream() << geometry.positions;
  stream() << geometry.normals;
  stream() << geometry.tangents;
  stream() << geometry.binormals;
  stream() << geometry.texCoords;
  uint64_t skeletonRefsNumber = geometry.skeletonRefs.size();
  stream() << skeletonRefsNumber;
  for (const mtt::CommonMeshGeometry::SkeletonRef& skeletonRef :
                                                        geometry.skeletonRefs)
  {
    stream() << skeletonRef.bonesNumber;
    for ( uint16_t boneIndex = 0;
          boneIndex < skeletonRef.bonesNumber;
          boneIndex++)
    {
      stream() << skeletonRef.bones[boneIndex].boneIndex;
      stream() << skeletonRef.bones[boneIndex].weight;
    }
  }

  stream() << geometry.triangleIndices;
  stream() << geometry.lineIndices;
}

void ObjectSaver::saveBoneRefs(const BoneRefBatch& refs)
{
  stream() << (uint16_t) refs.boneRefsNumber();
  for (size_t refIndex = 0; refIndex < refs.boneRefsNumber(); refIndex++)
  {
    const BoneRefObject& boneRef = refs.boneRef(refIndex);
    stream() << boneRef.name();
    stream() << boneRef.boneRef().referencedId();
    stream() << boneRef.boneInverseMatrix();
  }
}

void ObjectSaver::visitConstRootObject(const RootObject& object)
{
  OEVisitorT::visitConstRootObject(object);

  saveObjectData( object.materialsGroup(),
                  stream(),
                  fileDirectory(),
                  objectFactory());

  saveObjectData( object.skeletonGroup(),
                  stream(),
                  fileDirectory(),
                  objectFactory());

  saveObjectData( object.geometryGroup(),
                  stream(),
                  fileDirectory(),
                  objectFactory());

  saveObjectData( object.animationGroup(),
                  stream(),
                  fileDirectory(),
                  objectFactory());
}
