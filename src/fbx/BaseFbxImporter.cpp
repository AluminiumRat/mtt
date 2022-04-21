#include <stdexcept>

#include <QtCore/QFileInfo>

#include <mtt/fbx/BaseFbxImporter.h>
#include <mtt/fbx/FbxLib.h>
#include <mtt/fbx/FbxRAII.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

const BaseFbxImporter::MaterialOptions BaseFbxImporter::blenderMaterialOptions{
                                                                  false,
                                                                  true,
                                                                  true};

const BaseFbxImporter::MaterialOptions BaseFbxImporter::maxMaterialOptions{
                                                              false,
                                                              false,
                                                              false};

BaseFbxImporter::BaseFbxImporter():
  _normalLayer(nullptr),
  _tangentLayer(nullptr),
  _binormalLayer(nullptr),
  _texCoordLayer(nullptr),
  _materialLayer(nullptr)
{
}

void BaseFbxImporter::startImporting(const char* filename)
{
  _fileDir = QFileInfo(filename).absoluteDir();

  FbxManager* manager = &FbxLib::instance().fbxManager();
  FbxContainer<FbxScene> scene(manager, "scene");

  {
    FbxContainer<FbxImporter> importer(manager, "");
    if(!importer().Initialize(filename, -1, manager->GetIOSettings()))
    {
      std::string errorString("Unable to initialize fbx importer: ");
      throw std::runtime_error(errorString + importer().GetStatus().GetErrorString());
    }

    if (!importer().Import(&scene()))
    {
      std::string errorString("Unable to import scene: ");
      throw std::runtime_error(errorString + importer().GetStatus().GetErrorString());
    }
  }

  FbxSystemUnit::m.ConvertScene(&scene());
  FbxAxisSystem axisSystem;
  FbxAxisSystem::ParseAxisSystem("xzy", axisSystem);
  axisSystem.ConvertScene(&scene());

  processScene(scene());

  FbxNode* rootNode = scene().GetRootNode();
  if(rootNode != nullptr) _importNode(*rootNode);
}

void BaseFbxImporter::processScene(FbxScene& scene)
{
}

void BaseFbxImporter::_importNode(FbxNode& node)
{
  FBXMaterials oldMaterials = _materials;

  _updateMaterials(node);

  pushTranslation(node);

  FbxMesh* mesh = node.GetMesh();
  if(mesh != nullptr) _importMesh(*mesh);

  for(int childIndex = 0;
      childIndex < node.GetChildCount();
      childIndex++)
  {
    _importNode(*node.GetChild(childIndex));
  }

  popTranslation();
  _materials = oldMaterials;
}

void BaseFbxImporter::_updateMaterials(FbxNode& node)
{
  if (node.GetMaterialCount() == 0) return;

  _materials.clear();
  for(int materialIndex = 0;
      materialIndex < node.GetMaterialCount();
      materialIndex++)
  {
    _materials.push_back(node.GetMaterial(materialIndex));
  }
}

void BaseFbxImporter::pushTranslation(FbxNode& node)
{
}

void BaseFbxImporter::popTranslation()
{
}

void BaseFbxImporter::_importMesh(FbxMesh& mesh)
{
  _meshData.clear();
  _meshData.resize(_materials.size());

  _readLayers(mesh);
  _readSkinData(mesh);
  _distributePoints(mesh);
  
  for(size_t materialIndex = 0;
      materialIndex < _materials.size();
      materialIndex++)
  {
    if(!_meshData[materialIndex].positions.empty())
    {
      processMesh(std::move(_meshData[materialIndex]),
                  *_materials[materialIndex],
                  _bones,
                  mesh);
    }
  }

  _meshData.clear();
}

void BaseFbxImporter::_readLayers(FbxMesh& mesh)
{
  if(mesh.GetLayerCount() != 1) throw std::runtime_error("Wrong number of layers in fbx mesh.");
  FbxLayer* layer = mesh.GetLayer(0);

  _normalLayer = layer->GetNormals();
  if(_normalLayer == nullptr) throw std::runtime_error("Normal data not found in fbx mesh.");
  if( _normalLayer->GetMappingMode() != FbxLayerElement::eByControlPoint &&
      _normalLayer->GetMappingMode() != FbxLayerElement::eByPolygonVertex &&
      _normalLayer->GetMappingMode() != FbxLayerElement::eByPolygon)
  {
    throw std::runtime_error("Wrong normal mapping in fbx mesh.");
  }

  _tangentLayer = layer->GetTangents();
  if( _tangentLayer != nullptr &&
      _tangentLayer->GetMappingMode() != FbxLayerElement::eByControlPoint &&
      _tangentLayer->GetMappingMode() != FbxLayerElement::eByPolygonVertex &&
      _tangentLayer->GetMappingMode() != FbxLayerElement::eByPolygon)
  {
    throw std::runtime_error("Wrong tangent mapping in fbx mesh.");
  }

  _binormalLayer = layer->GetBinormals();
  if( _binormalLayer != nullptr &&
      _binormalLayer->GetMappingMode() != FbxLayerElement::eByControlPoint &&
      _binormalLayer->GetMappingMode() != FbxLayerElement::eByPolygonVertex &&
      _binormalLayer->GetMappingMode() != FbxLayerElement::eByPolygon)
  {
    throw std::runtime_error("Wrong binormal mapping in fbx mesh.");
  }

  _texCoordLayer = layer->GetUVs();
  if( _texCoordLayer != nullptr &&
      _texCoordLayer->GetMappingMode() != FbxLayerElement::eByControlPoint &&
      _texCoordLayer->GetMappingMode() != FbxLayerElement::eByPolygonVertex)
  {
    throw std::runtime_error("Wrong uv mapping in fbx mesh.");
  }

  _materialLayer = layer->GetMaterials();
  if( _materialLayer == nullptr) throw std::runtime_error("Material data not found in fbx mesh.");
  if( _materialLayer->GetMappingMode() != FbxLayerElement::eByPolygon &&
      _materialLayer->GetMappingMode() != FbxLayerElement::eAllSame)
  {
    throw std::runtime_error("Wrong material mapping in fbx mesh.");
  }
}

void BaseFbxImporter::_readSkinData(FbxMesh& mesh)
{
  _bones.clear();
  _skeletonRefs.clear();

  if (mesh.GetControlPointsCount() == 0) return;
  _skeletonRefs.resize(mesh.GetControlPointsCount());

  if(mesh.GetDeformerCount() == 0) return;
  if (mesh.GetDeformerCount() > 1)
  {
    throw std::runtime_error("Only one deformer per mesh was supported.");
  }
  FbxDeformer* deformer = mesh.GetDeformer(0);
  if(deformer->GetDeformerType() != FbxDeformer::eSkin)
  {
    throw std::runtime_error("Only skin deformer was supported.");
  }
  FbxSkin* skin = static_cast<FbxSkin*>(deformer);

  for(int clusterIndex = 0;
      clusterIndex < skin->GetClusterCount();
      clusterIndex++)
  {
    FbxCluster* cluster = skin->GetCluster(clusterIndex);
    uint16_t boneIndex = _getOrCreateBone(*cluster);

    int* indices = cluster->GetControlPointIndices();
    double* weights = cluster->GetControlPointWeights();
    for(int pointIndex = 0;
        pointIndex < cluster->GetControlPointIndicesCount();
        pointIndex++)
    {
      BoneRef newRef;
      newRef.boneIndex = boneIndex;
      newRef.weight = weights[pointIndex];

      SkeletonRef& skeletonRef = _skeletonRefs[indices[pointIndex]];
      if(skeletonRef.bonesNumber <CommonMeshGeometry::MAX_BONES_PER_VERTEX)
      {
        skeletonRef.bones[skeletonRef.bonesNumber] = newRef;
        skeletonRef.bonesNumber++;
      }
      else
      {
        Log() << "BaseFbxImporter::_readSkinData: Too many bones attached to the vertex";
      }
    }
  }
}

uint16_t BaseFbxImporter::_getOrCreateBone(FbxCluster& cluster)
{
  FbxNode* boneNode = cluster.GetLink();
  uint16_t boneIndex = 0;
  for(; boneIndex < _bones.size(); boneIndex++)
  {
    if(_bones[boneIndex].node == boneNode) break;
  }
  if(boneIndex == _bones.size())
  {
    Bone newBone;
    newBone.node = boneNode;

    FbxAMatrix meshInitTransformFbx;
    cluster.GetTransformMatrix(meshInitTransformFbx);
    glm::mat4 meshInitTransform = toGlm(meshInitTransformFbx);

    FbxAMatrix boneInitTransformFbx;
    cluster.GetTransformLinkMatrix(boneInitTransformFbx);
    glm::mat4 boneInitTransform = toGlm(boneInitTransformFbx);

    newBone.toBone = glm::inverse(boneInitTransform) * meshInitTransform;

    _bones.push_back(newBone);
  }
  return boneIndex;
}

glm::mat4 BaseFbxImporter::toGlm(const FbxAMatrix& matrix) noexcept
{
  glm::mat4 result;

  result[0] = glm::vec4(matrix.Get(0, 0),
                        matrix.Get(0, 1),
                        matrix.Get(0, 2),
                        matrix.Get(0, 3));
  result[1] = glm::vec4(matrix.Get(1, 0),
                        matrix.Get(1, 1),
                        matrix.Get(1, 2),
                        matrix.Get(1, 3));
  result[2] = glm::vec4(matrix.Get(2, 0),
                        matrix.Get(2, 1),
                        matrix.Get(2, 2),
                        matrix.Get(2, 3));
  result[3] = glm::vec4(matrix.Get(3, 0),
                        matrix.Get(3, 1),
                        matrix.Get(3, 2),
                        matrix.Get(3, 3));

  return result;
}

void BaseFbxImporter::_distributePoints(FbxMesh& mesh)
{
  int polygonCount = mesh.GetPolygonCount();
  int polygonVertexProcessed = 0;
  for(int polygonIndex = 0;
      polygonIndex < polygonCount;
      polygonIndex++)
  {
    int materialIndex = _materialLayer->GetIndexArray()[0];
    if(_materialLayer->GetMappingMode() == FbxLayerElement::eByPolygon)
    {
      materialIndex = _materialLayer->GetIndexArray()[polygonIndex];
    }

    int polygonSize = mesh.GetPolygonSize(polygonIndex);
    if (polygonSize < 1) throw std::runtime_error("Fbx file error: the number of vertices in the polygon is less than 1.");

    _addPolygonIndices(materialIndex, polygonSize);

    for(int inPolygonIndex = 0; inPolygonIndex < polygonSize; inPolygonIndex++)
    {
      int globalIndex = mesh.GetPolygonVertex(polygonIndex, inPolygonIndex);
      int polygonVertexIndex = polygonVertexProcessed + inPolygonIndex;
      _addVertex( mesh,
                  materialIndex,
                  globalIndex,
                  polygonVertexIndex,
                  polygonIndex,
                  inPolygonIndex);
    }
  
    polygonVertexProcessed += polygonSize;
  }
}

void BaseFbxImporter::_addPolygonIndices(int materialIndex, int polygonSize)
{
  int startPolygonVertex = int(_meshData[materialIndex].positions.size());

  for(int triangleIndex = 0;
      triangleIndex < polygonSize - 2;
      triangleIndex++)
  {
    _meshData[materialIndex].triangleIndices.push_back(startPolygonVertex);
    _meshData[materialIndex].triangleIndices.push_back(
                                        startPolygonVertex + triangleIndex + 2);
    _meshData[materialIndex].triangleIndices.push_back(
                                        startPolygonVertex + triangleIndex + 1);
  }

  for(int lineIndex = 0; lineIndex < polygonSize - 1; lineIndex++)
  {
    _meshData[materialIndex].lineIndices.push_back(
                                                startPolygonVertex + lineIndex);
    _meshData[materialIndex].lineIndices.push_back(
                                            startPolygonVertex + lineIndex + 1);
  }
  if(polygonSize > 2)
  {
    _meshData[materialIndex].lineIndices.push_back(startPolygonVertex);
    _meshData[materialIndex].lineIndices.push_back(startPolygonVertex +
                                                              polygonSize - 1);
  }
}

template <typename ElementType>
static ElementType getElement(FbxLayerElementTemplate<ElementType>& layer,
                              int globalIndex,
                              int polygonVertexIndex,
                              int polygonIndex)
{
  int elementIndex = 0;
  switch(layer.GetMappingMode())
  {
    case FbxLayerElement::eByControlPoint:
      elementIndex = globalIndex;
      break;
    case FbxLayerElement::eByPolygonVertex:
      elementIndex = polygonVertexIndex;
      break;
    case FbxLayerElement::eByPolygon:
      elementIndex = polygonIndex;
      break;
    default:
      Abort("BaseFbxImporter::getElement: Wrong mapping mode.");
  }
  if(layer.GetReferenceMode() == FbxLayerElement::eIndexToDirect)
  {
    elementIndex = layer.GetIndexArray()[elementIndex];
  }
  return layer.GetDirectArray()[elementIndex];
}

void BaseFbxImporter::_addVertex( FbxMesh& mesh,
                                  int materialIndex,
                                  int globalIndex,
                                  int polygonVertexIndex,
                                  int polygonIndex,
                                  int indexInPolygon)
{
  CommonMeshGeometry& submesh = _meshData[materialIndex];

  FbxVector4 fbxPosition = mesh.GetControlPointAt(globalIndex);
  submesh.positions.push_back(glm::vec3(fbxPosition[0],
                                        fbxPosition[1],
                                        fbxPosition[2]));
  
  FbxVector4 fbxNormal = getElement<FbxVector4>(*_normalLayer,
                                                globalIndex,
                                                polygonVertexIndex,
                                                polygonIndex);
  submesh.normals.push_back(glm::vec3(fbxNormal[0],
                                      fbxNormal[1],
                                      fbxNormal[2]));

  if(_tangentLayer != nullptr)
  {
    FbxVector4 fbxTangent = getElement<FbxVector4>( *_tangentLayer,
                                                    globalIndex,
                                                    polygonVertexIndex,
                                                    polygonIndex);
    submesh.tangents.push_back(glm::vec3( fbxTangent[0],
                                          fbxTangent[1],
                                          fbxTangent[2]));
  }

  if(_binormalLayer != nullptr)
  {
    FbxVector4 fbxBinormal = getElement<FbxVector4>( *_binormalLayer,
                                                    globalIndex,
                                                    polygonVertexIndex,
                                                    polygonIndex);
    submesh.binormals.push_back(glm::vec3(fbxBinormal[0],
                                          fbxBinormal[1],
                                          fbxBinormal[2]));
  }

  if(_texCoordLayer != nullptr)
  {
    FbxVector2 fbxTexCoord = getElement<FbxVector2>(*_texCoordLayer,
                                                    globalIndex,
                                                    polygonVertexIndex,
                                                    polygonIndex);
    submesh.texCoords.push_back(glm::vec2(fbxTexCoord[0],
                                          1.f - fbxTexCoord[1]));
  }

  if (!_skeletonRefs.empty())
  {
    submesh.skeletonRefs.push_back(_skeletonRefs[globalIndex]);
  }
}

void BaseFbxImporter::processMesh(CommonMeshGeometry&& vertices,
                                  const FbxSurfaceMaterial& material,
                                  const Bones& bones,
                                  const FbxMesh& mesh)
{
}

QString BaseFbxImporter::textureFullFilePath(const FbxProperty& theProperty)
{
  int textureCount = theProperty.GetSrcObjectCount<FbxTexture>();
  if(textureCount == 0) return QString();
  if(textureCount > 1) throw std::runtime_error("The import of multiple textures per property is not supported.");

  FbxLayeredTexture* layeredTexture =
                                theProperty.GetSrcObject<FbxLayeredTexture>(0);
  if(layeredTexture != nullptr) throw std::runtime_error("The import of layered textures is not supported.");
  FbxFileTexture* fbxTexture = theProperty.GetSrcObject<FbxFileTexture>(0);
  if(fbxTexture == nullptr) return QString();

  QString relativeFilename = fbxTexture->GetRelativeFileName();
  if(relativeFilename.isEmpty())
  {
    return fbxTexture->GetFileName();
  }
  return _fileDir.filePath(relativeFilename);
}

BaseFbxImporter::MaterialDescription
    BaseFbxImporter::getMaterialDescription(const FbxSurfaceMaterial& material,
                                            const MaterialOptions& options)
{
  MaterialDescription description;

  const FbxSurfaceLambert* lambert = nullptr;
  const FbxSurfacePhong* phong = nullptr;
  if(material.GetClassId().Is(FbxSurfacePhong::ClassId))
  {
    lambert = static_cast<const FbxSurfaceLambert*>(&material);
    phong = static_cast<const FbxSurfacePhong*>(&material);
  }
  if (material.GetClassId().Is(FbxSurfaceLambert::ClassId))
  {
    lambert = static_cast<const FbxSurfaceLambert*>(&material);
  }
  if(lambert == nullptr) throw std::runtime_error("Unsupported material in fbx model.");

  description.normalTextureFilename = textureFullFilePath(lambert->NormalMap);
  if(description.normalTextureFilename.isEmpty())
  {
    description.normalTextureFilename = textureFullFilePath(lambert->Bump);
  }

  QString emissiveTexture = textureFullFilePath(lambert->Emissive);
  if(!emissiveTexture.isEmpty())
  {
    if(options.specularMapFromEmission)
    {
      description.specularTextureFilename = emissiveTexture;
    }
  }

  QString diffuseTexture = textureFullFilePath(lambert->Diffuse);
  if(!diffuseTexture.isEmpty())
  {
    description.albedoTextureFilename = diffuseTexture;
    description.materialData.albedo = glm::vec3(lambert->DiffuseFactor.Get());
  }
  else
  {
    FbxDouble3 fbxDiffuse = lambert->Diffuse.Get();
    description.materialData.albedo = glm::vec3(fbxDiffuse[0],
                                                fbxDiffuse[1],
                                                fbxDiffuse[2]);
    description.materialData.albedo *= lambert->DiffuseFactor.Get();
  }

  description.materialData.emission = glm::vec3(lambert->EmissiveFactor.Get());
  if (!emissiveTexture.isEmpty() && !options.specularMapFromEmission)
  {
    description.emissionTextureFilename = emissiveTexture;
  }
  else
  {
    FbxDouble3 fbxEmissive = lambert->Emissive.Get();
    description.materialData.emission *= glm::vec3( fbxEmissive[0],
                                                    fbxEmissive[1],
                                                    fbxEmissive[2]);
  }

  description.materialData.opaque = 1.f - lambert->TransparencyFactor.Get();
  QString opaqueTexture = textureFullFilePath(lambert->TransparencyFactor);
  if(opaqueTexture.isEmpty())
  {
    opaqueTexture = textureFullFilePath(lambert->TransparentColor);
  }
  if(!opaqueTexture.isEmpty())
  {
    description.opaqueTextureFilename = opaqueTexture;
  }

  if(phong != nullptr)
  {
    float shininess = phong->Shininess.Get();
    if (options.blenderSpecular)
    {
      description.materialData.roughness = 1.f - sqrt(shininess / 100.f);
    }
    else
    {
      description.materialData.roughness = sqrt(sqrt(2.f / (shininess + 2.f)));
    }

    description.materialData.specularStrength = phong->SpecularFactor.Get();
    if(options.blenderSpecular)
    {
      description.materialData.specularStrength *= 2;
    }

    if (!options.specularMapFromEmission)
    {
      description.specularTextureFilename =
                                          textureFullFilePath(phong->Specular);
      if(description.specularTextureFilename.isEmpty())
      {
        description.specularTextureFilename =
                                    textureFullFilePath(phong->SpecularFactor);
      }
    }

    if(options.metallicFromReflectionFactor)
    {
      description.materialData.metallic = phong->ReflectionFactor.Get();
    }
    else
    {
      description.materialData.metallic = 0;

      description.reflectionTextureFilename =
                                        textureFullFilePath(phong->Reflection);
      if (!description.reflectionTextureFilename.isEmpty())
      {
        description.materialData.reflectionFactor =
                                                  phong->ReflectionFactor.Get();
      }
    }
  }
  else
  {
    description.materialData.roughness = 1;
    description.materialData.specularStrength = 0;
    description.materialData.metallic = 0;
  }

  return description;
}

std::set<FbxTime> BaseFbxImporter::getKeypointTimes(FbxNode& node,
                                                    FbxAnimLayer& layer)
{
  std::set<FbxTime> timeSet;
  FbxAnimCurve* curve = node.LclTranslation.GetCurve(
                                                  &layer,
                                                  FBXSDK_CURVENODE_COMPONENT_X);
  if(curve != nullptr) fillKeypointTimes(timeSet, *curve);

  curve = node.LclTranslation.GetCurve(&layer,
                                       FBXSDK_CURVENODE_COMPONENT_Y);
  if(curve != nullptr) fillKeypointTimes(timeSet, *curve);

  curve = node.LclTranslation.GetCurve(&layer,
                                       FBXSDK_CURVENODE_COMPONENT_Z);
  if(curve != nullptr) fillKeypointTimes(timeSet, *curve);

  curve = node.LclRotation.GetCurve(&layer,
                                    FBXSDK_CURVENODE_COMPONENT_X);
  if (curve != nullptr) fillKeypointTimes(timeSet, *curve);

  curve = node.LclRotation.GetCurve(&layer,
                                    FBXSDK_CURVENODE_COMPONENT_Y);
  if (curve != nullptr) fillKeypointTimes(timeSet, *curve);

  curve = node.LclRotation.GetCurve(&layer,
                                    FBXSDK_CURVENODE_COMPONENT_Z);
  if (curve != nullptr) fillKeypointTimes(timeSet, *curve);

  return timeSet;
}

void BaseFbxImporter::fillKeypointTimes(std::set<FbxTime>& timeSet,
                                        FbxAnimCurve& curve)
{
  for(int keyIndex = 0; keyIndex < curve.KeyGetCount(); keyIndex++)
  {
    timeSet.insert(curve.KeyGetTime(keyIndex));
  }
}
