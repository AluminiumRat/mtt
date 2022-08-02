#include <QtCore/QCoreApplication>

#include <glm/gtc/quaternion.hpp>

#include <mtt/utilities/Log.h>

#include <FbxDrawableImporter.h>

FbxDrawableImporter::Result FbxDrawableImporter::import(
                                        const char* filename,
                                        const MaterialOptions& materialOptions)
{
  _materialOptions = materialOptions;

  _skeletonMap.clear();

  _result.skeleton.clear();
  _result.drawables.clear();

  startImporting(filename);

  _resolveBoneLinks();

  return std::move(_result);
}

void FbxDrawableImporter::pushTranslation(FbxNode& node)
{
  std::unique_ptr<mtt::SkeletonObject> newSkeleton(
                                new mtt::SkeletonObject(node.GetName(), true));
  mtt::SkeletonObject& skeletonRef = *newSkeleton;
  _skeletonMap[&node] = &skeletonRef;

  FbxDouble3 position = node.LclTranslation.Get();
  newSkeleton->setPosition(glm::vec3(position[0], position[1], position[2]));

  FbxDouble3 rotation = node.LclRotation.Get();
  newSkeleton->setRotation(glm::quat( glm::vec3(glm::radians(rotation[0]),
                                                glm::radians(rotation[1]),
                                                glm::radians(rotation[2]))));
  
  FbxDouble3 scaling = node.LclScaling.Get();
  newSkeleton->setScale(glm::vec3(scaling[0], scaling[1], scaling[2]));

  if(_skeletonStack.empty()) _result.skeleton.push_back(std::move(newSkeleton));
  else _skeletonStack.back()->addChild(std::move(newSkeleton));
  _skeletonStack.push_back(&skeletonRef);
}

void FbxDrawableImporter::popTranslation()
{
  _skeletonStack.pop_back();
}

void FbxDrawableImporter::processMesh(mtt::CommonMeshGeometry&& vertices,
                                      const FbxSurfaceMaterial& fbxMaterial,
                                      const Bones& bones,
                                      const FbxMesh& mesh)
{
  MaterialObject* material = nullptr;
  MaterialMap::iterator iMaterial = _materialMap.find(&fbxMaterial);
  if(iMaterial != _materialMap.end()) material = iMaterial->second;
  else
  {
    std::unique_ptr<MaterialObject> newMaterial(
                              new MaterialObject(fbxMaterial.GetName(), true));
    _importMaterial(*newMaterial, fbxMaterial);
    material = newMaterial.get();
    _result.materials.push_back(std::move(newMaterial));
  }

  std::unique_ptr<MeshObject> newDrawable(new MeshObject(mesh.GetName(), true));
  newDrawable->setGeometry(std::move(vertices));

  newDrawable->materialRef().set(material);

  if(!_skeletonStack.empty())
  {
    newDrawable->skeletonRef().set(_skeletonStack.back());
  }

  if(!bones.empty())_boneLinks.push_back({newDrawable.get(), bones});

  _result.drawables.push_back(std::move(newDrawable));
}

void FbxDrawableImporter::_importMaterial(MaterialObject& meshMaterial,
                                          const FbxSurfaceMaterial& fbxMaterial)
{
  MaterialDescription materialDescription =
                                      getMaterialDescription( fbxMaterial,
                                                              _materialOptions);

  meshMaterial.setAlbedo(materialDescription.materialData.albedo);
  meshMaterial.setRoughness(materialDescription.materialData.roughness);
  meshMaterial.setSpecularStrength(
                            materialDescription.materialData.specularStrength);
  meshMaterial.setMetallic(materialDescription.materialData.metallic);
  meshMaterial.setOpaqueFactor(materialDescription.materialData.opaque);
  meshMaterial.setReflectionFactor(
                            materialDescription.materialData.reflectionFactor);
  if(materialDescription.materialData.emission == glm::vec3(0))
  {
    meshMaterial.setEmissionColor(glm::vec3(0));
    meshMaterial.setEmissionFactor(0);
  }
  else
  {
    meshMaterial.setEmissionColor(
                    glm::normalize(materialDescription.materialData.emission));
    meshMaterial.setEmissionFactor(
                        glm::length(materialDescription.materialData.emission));
  }

  meshMaterial.setAlbedoTexture(materialDescription.albedoTextureFilename);
  meshMaterial.setSpecularTexture(materialDescription.specularTextureFilename);
  meshMaterial.setNormalTexture(materialDescription.normalTextureFilename);
  meshMaterial.setOpaqueTexture(materialDescription.opaqueTextureFilename);
  meshMaterial.setEmissionTexture(materialDescription.emissionTextureFilename);
  meshMaterial.setReflectionTexture(
                                materialDescription.reflectionTextureFilename);
}

void FbxDrawableImporter::_resolveBoneLinks()
{
  for(BonesLink& link : _boneLinks)
  {
    std::vector<std::unique_ptr<BoneRefObject>> references;
    for(Bone& bone : link.bones)
    {
      SkeletonMap::const_iterator iBone = _skeletonMap.find(bone.node);
      if(iBone == _skeletonMap.end()) throw std::runtime_error("FbxDrawableImporter: Unable to resolve bone references.");
      mtt::SkeletonObject* skeleton = iBone->second;

      std::unique_ptr<BoneRefObject> newRef(
                              new BoneRefObject(skeleton->name(), true));
      newRef->setBoneInverseMatrix(bone.toBone);
      newRef->boneRef().set(skeleton);

      references.push_back(std::move(newRef));
    }

    link.mesh->setBoneRefs(std::make_unique<BoneRefBatch>(
                                        std::move(references),
                                        QCoreApplication::tr("Bone references"),
                                        false));
  }
}
