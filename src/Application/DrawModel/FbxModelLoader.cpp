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
  std::unique_ptr<MasterDrawModel> model(new MasterDrawModel());
  _model = model.get();

  startImporting(_filename.toUtf8().data());

  return model;
}

void FbxModelLoader::pushTranslation(FbxNode& node)
{
  TransformTable::Index boneIndex = _model->transformTable().addBone();

  std::unique_ptr<Joint> newJoint(new Joint);
  Joint& jointRef = *newJoint;

  _model->addJoint( std::move(newJoint),
                    boneIndex,
                    node.GetName());

  glm::mat4 transform = toGlm(node.EvaluateLocalTransform());
  jointRef.setJointMatrix(transform);

  if(!_jointsStack.empty()) _jointsStack.back()->addChild(jointRef);
  _jointsStack.push_back(&jointRef);
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

  MaterialDescription material = getMaterialDescription(fbxMaterial,
                                                        _materialOptions);

  MeshExtraData& meshExtraData = meshRef.mesh().extraData();

  meshExtraData.setSurfaceMaterialData(material.materialData);

  if(!material.albedoTextureFilename.isEmpty())
  {
    meshExtraData.setAlbedoSampler(loadTexture(material.albedoTextureFilename));
  }

  if (!material.opaqueTextureFilename.isEmpty())
  {
    meshExtraData.setOpaqueSampler(loadTexture(material.opaqueTextureFilename));
  }

  if (!material.specularTextureFilename.isEmpty())
  {
    meshExtraData.setSpecularSampler(
                                loadTexture(material.specularTextureFilename));
  }

  if (!material.normalTextureFilename.isEmpty())
  {
    meshExtraData.setNormalSampler(loadTexture(material.normalTextureFilename));
  }

  if (!material.emissionTextureFilename.isEmpty())
  {
    meshExtraData.setEmissionSampler(
                                loadTexture(material.emissionTextureFilename));
  }

  if (!material.reflectionTextureFilename.isEmpty())
  {
    meshExtraData.setReflectionSampler(
                              loadTexture(material.reflectionTextureFilename));
  }

  _techniquesFactory.setupTechniques(meshRef.mesh());
}

std::unique_ptr<Sampler> FbxModelLoader::loadTexture(const QString& fileName)
{
  std::unique_ptr<Sampler> sampler(new Sampler( PipelineResource::STATIC,
                                                _device));
  std::shared_ptr<Texture2D> texture =
                                  _textureLibrary.load(fileName, _device, true);
  sampler->setAttachedTexture(texture);
  return sampler;
}
