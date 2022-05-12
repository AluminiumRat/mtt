#include <Objects/ObjectLoader.h>

void ObjectLoader::visitGeometryGroup(GeometryGroup& object)
{
  OEVisitorT::visitGeometryGroup(object);
  readChilds<GeometryGroup, LODObject>(object, true);
}

void ObjectLoader::visitGeometryObject(GeometryObject& object)
{
  OEVisitorT::visitGeometryObject(object);
  object.skeletonRef().setReferencedId(readUID());
}

void ObjectLoader::visitLODObject(LODObject& object)
{
  OEVisitorT::visitLODObject(object);
  object.setMinMppx(stream().readFloat());
  object.setMaxMppx(stream().readFloat());

  uint32_t childNumber = stream().readUint32();
  for (; childNumber != 0; childNumber--)
  {
    object.addChild(loadObject<MeshObject>( true,
                                            stream(),
                                            fileDirectory(),
                                            mixUIDValue(),
                                            objectFactory()));
  }
}

void ObjectLoader::visitMaterialsGroup(MaterialsGroup& object)
{
  OEVisitorT::visitMaterialsGroup(object);
  readChilds<MaterialsGroup, MaterialObject>(object, true);
}

void ObjectLoader::visitMaterialObject(MaterialObject& object)
{
  OEVisitorT::visitMaterialObject(object);

  object.setAlbedo(stream().readVec3());
  object.setAlbedoTexture(readFilename());
  object.setUseAlphaFromAlbedoTexture(stream().readBool());

  object.setRoughness(stream().readFloat());
  object.setSpecularStrength(stream().readFloat());
  object.setSpecularTexture(readFilename());

  object.setMetallic(stream().readFloat());

  object.setOpaqueFactor(stream().readFloat());
  object.setOpaqueTexture(readFilename());

  object.setReflectionFactor(stream().readFloat());
  object.setReflectionTexture(readFilename());

  object.setEmissionColor(stream().readVec3());
  object.setEmissionFactor(stream().readFloat());
  object.setEmissionTexture(readFilename());

  object.setNormalTexture(readFilename());
}

void ObjectLoader::readGeometry(mtt::CommonMeshGeometry& geometry)
{
  stream() >> geometry.positions;
  stream() >> geometry.normals;
  stream() >> geometry.tangents;
  stream() >> geometry.binormals;
  stream() >> geometry.texCoords;
  uint64_t skeletonRefsNumber = stream().readUint64();
  geometry.skeletonRefs.resize(skeletonRefsNumber);
  for (mtt::CommonMeshGeometry::SkeletonRef& skeletonRef :
                                                        geometry.skeletonRefs)
  {
    stream() >> skeletonRef.bonesNumber;
    for ( uint16_t boneIndex = 0;
          boneIndex < skeletonRef.bonesNumber;
          boneIndex++)
    {
      stream() >> skeletonRef.bones[boneIndex].boneIndex;
      stream() >> skeletonRef.bones[boneIndex].weight;
    }
  }

  stream() >> geometry.triangleIndices;
  stream() >> geometry.lineIndices;
}

std::unique_ptr<BoneRefBatch> ObjectLoader::readBoneRefs()
{
  std::vector<std::unique_ptr<BoneRefObject>> refs;

  uint16_t boneRefsNumber = stream().readUint16();
  for (;boneRefsNumber != 0; boneRefsNumber--)
  {
    QString name;
    stream() >> name;

    std::unique_ptr<BoneRefObject> refObject(new BoneRefObject(name, true));
    refObject->boneRef().setReferencedId(readUID());
    refObject->setBoneInverseMatrix(stream().readMat4());
    refs.push_back(std::move(refObject));
  }

  return std::make_unique<BoneRefBatch>(
                                      std::move(refs),
                                      QObject::tr("Bone references"),
                                      false);
}

void ObjectLoader::visitMeshObject(MeshObject& object)
{
  OEVisitorT::visitMeshObject(object);

  mtt::CommonMeshGeometry geometry;
  readGeometry(geometry);
  object.setGeometry(std::move(geometry));
  object.setBoneRefs(readBoneRefs());
  object.materialRef().setReferencedId(readUID());
}

void ObjectLoader::visitRootObject(RootObject& object)
{
  OEVisitorT::visitRootObject(object);

  loadEmbeddedObject( object.materialsGroup(),
                      stream(),
                      fileDirectory(),
                      mixUIDValue(),
                      objectFactory());

  loadEmbeddedObject( object.skeletonGroup(),
                      stream(),
                      fileDirectory(),
                      mixUIDValue(),
                      objectFactory());

  loadEmbeddedObject( object.geometryGroup(),
                      stream(),
                      fileDirectory(),
                      mixUIDValue(),
                      objectFactory());

  loadEmbeddedObject( object.animationGroup(),
                      stream(),
                      fileDirectory(),
                      mixUIDValue(),
                      objectFactory());
}
