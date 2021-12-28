#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <mtt/Application/DrawModel/FbxModelLoader.h>
#include <mtt/Application/ResourceManager/Texture2DLibrary.h>
#include <mtt/Render/Mesh/MeshTechniquesFactory.h>
#include <mtt/Render/Pipeline/Texture2D.h>

using namespace mtt;

FbxModelLoader::FbxModelLoader(
                              const QString& filename,
                              BaseFbxImporter::MaterialOptions materialOptions,
                              MeshTechniquesFactory& techniqueFactory,
                              Texture2DLibrary& textureLibrary,
                              LogicalDevice& device) :
  _filename(filename),
  _materialOptions(materialOptions),
  _techniquesFactory(techniqueFactory),
  _textureLibrary(textureLibrary),
  _device(device)
{
}

std::unique_ptr<MasterDrawModel> FbxModelLoader::load()
{
  _jointsStack.clear();
  _jointsMap.clear();
  std::unique_ptr<MasterDrawModel> model(new MasterDrawModel());
  _model = model.get();

  startImporting(_filename.toUtf8().data());

  for ( size_t meshIndex = 0;
        meshIndex < model->meshNodeNumber();
        meshIndex++)
  {
    model->getMeshNode(meshIndex).updateSkinFromBones();
  }

  return model;
}

void FbxModelLoader::pushTranslation(FbxNode& node)
{
  Joint& joint = _getOrCreateJoint(node);
  if(!_jointsStack.empty()) _jointsStack.back()->addChild(joint);
  _jointsStack.push_back(&joint);
}

Joint& FbxModelLoader::_getOrCreateJoint(FbxNode& node)
{
  JointsMap::iterator iJoint = _jointsMap.find(&node);
  if(iJoint != _jointsMap.end()) return *iJoint->second;

  TransformTable::Index boneIndex = _model->transformTable().addBone();

  std::unique_ptr<Joint> newJoint(new Joint);

  FbxDouble3 fbxPosition = node.LclTranslation.Get();
  glm::vec3 position(fbxPosition[0], fbxPosition[1], fbxPosition[2]);

  FbxDouble3 fbxRotation = node.LclRotation.Get();
  glm::quat rotation(glm::vec3( glm::radians(fbxRotation[0]),
                                glm::radians(fbxRotation[1]),
                                glm::radians(fbxRotation[2])));
  FbxDouble3 fbxScaling = node.LclScaling.Get();
  glm::vec3 scaling(fbxScaling[0], fbxScaling[1], fbxScaling[2]);

  glm::mat4 transform = glm::translate(glm::mat4(1), position) *
                        glm::mat4_cast(rotation) *
                        glm::scale(glm::mat4(1), scaling);
  newJoint->setJointMatrix(transform);

  Joint& jointRef = *newJoint;
  _model->addJoint( std::move(newJoint),
                    boneIndex,
                    node.GetName());

  _jointsMap[&node] = &jointRef;

  return jointRef;
}

void FbxModelLoader::popTranslation()
{
  _jointsStack.pop_back();
}

void FbxModelLoader::processMesh( mtt::CommonMeshGeometry&& vertices,
                                  const FbxSurfaceMaterial& fbxMaterial,
                                  const Bones& bones,
                                  const FbxMesh& mesh)
{
  std::unique_ptr<MeshControlNode> meshNode(new MeshControlNode(_device));
  MeshControlNode& meshRef = *meshNode;

  meshRef.setLocalBoundSphere(
                        vertices.calculateBoundingBox().buildBoundingSphere());
  meshRef.mesh().setGeometry(std::move(vertices));

  _model->addMeshNode(std::move(meshNode));
  if(!_jointsStack.empty()) _jointsStack.back()->addChild(meshRef);

  if(!bones.empty())
  {
    SkinControlNode::BoneRefs boneRefs;
    for (const Bone& bone : bones)
    {
      SkinControlNode::BoneRefData reference;
      reference.joint = &_getOrCreateJoint(*bone.node);
      reference.inverseBoneMatrix = bone.toBone;
      boneRefs.push_back(reference);
    }
    meshRef.setBoneRefs(boneRefs);
  }

  _updateMaterial(meshRef.mesh().extraData(), fbxMaterial);

  _techniquesFactory.setupTechniques(meshRef.mesh());
}

void FbxModelLoader::_updateMaterial( MeshExtraData& extraData,
                                      const FbxSurfaceMaterial& fbxMaterial)
{
  MaterialDescription material = getMaterialDescription(fbxMaterial,
                                                        _materialOptions);

  extraData.setSurfaceMaterialData(material.materialData);

  if (!material.albedoTextureFilename.isEmpty())
  {
    extraData.setAlbedoSampler(_loadTexture(material.albedoTextureFilename));
  }

  if (!material.opaqueTextureFilename.isEmpty())
  {
    extraData.setOpaqueSampler(_loadTexture(material.opaqueTextureFilename));
  }

  if (!material.specularTextureFilename.isEmpty())
  {
    extraData.setSpecularSampler(
                                _loadTexture(material.specularTextureFilename));
  }

  if (!material.normalTextureFilename.isEmpty())
  {
    extraData.setNormalSampler(_loadTexture(material.normalTextureFilename));
  }

  if (!material.emissionTextureFilename.isEmpty())
  {
    extraData.setEmissionSampler(
                                _loadTexture(material.emissionTextureFilename));
  }

  if (!material.reflectionTextureFilename.isEmpty())
  {
    extraData.setReflectionSampler(
                              _loadTexture(material.reflectionTextureFilename));
  }
}

std::unique_ptr<Sampler> FbxModelLoader::_loadTexture(const QString& fileName)
{
  std::unique_ptr<Sampler> sampler(new Sampler( PipelineResource::STATIC,
                                                _device));
  std::shared_ptr<Texture2D> texture =
                                  _textureLibrary.load(fileName, _device, true);
  sampler->setAttachedTexture(texture);
  return sampler;
}
