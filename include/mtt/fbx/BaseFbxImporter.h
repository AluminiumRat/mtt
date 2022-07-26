#pragma once

#include <memory>
#include <set>
#include <vector>

#include <QtCore/QDir>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <fbxsdk.h>

#include <mtt/utilities/CommonMeshGeometry.h>
#include <mtt/utilities/SurfaceMaterialData.h>

namespace mtt
{
  class BaseFbxImporter
  {
  public:
    struct MaterialOptions
    {
      bool specularMapFromEmission = false;
      bool metallicFromReflectionFactor = true;
      bool blenderSpecular = true;
    };
    static const MaterialOptions blenderMaterialOptions;
    static const MaterialOptions maxMaterialOptions;

  public:
    BaseFbxImporter();
    BaseFbxImporter(const BaseFbxImporter&) = delete;
    BaseFbxImporter& operator = (const BaseFbxImporter&) = delete;
    virtual ~BaseFbxImporter() noexcept = default;

  protected:
    struct Bone
    {
      FbxNode* node = nullptr;
      glm::mat4 toBone = glm::mat4(1);
    };
    using Bones = std::vector<Bone>;

    struct MaterialDescription
    {
      SurfaceMaterialData materialData;
      QString albedoTextureFilename;
      QString specularTextureFilename;
      QString normalTextureFilename;
      QString opaqueTextureFilename;
      QString emissionTextureFilename;
      QString reflectionTextureFilename;
    };

  protected:
    void startImporting(const char* filename);

    virtual void processScene(FbxScene& scene);
    virtual void pushTranslation(FbxNode& node);
    virtual void popTranslation();
    virtual void processMesh( CommonMeshGeometry&& vertices,
                              const FbxSurfaceMaterial& material,
                              const Bones& bones,
                              const FbxMesh& mesh);

    static glm::mat4 toGlm(const FbxAMatrix& matrix) noexcept;
    MaterialDescription getMaterialDescription(
                                            const FbxSurfaceMaterial& material,
                                            const MaterialOptions& options);
    QString textureFullFilePath(const FbxProperty& theProperty);

    static std::set<FbxTime> getKeypointTimes(FbxNode& node,
                                              FbxAnimLayer& layer);

    static void fillKeypointTimes(std::set<FbxTime>& timeSet,
                                  FbxAnimCurve& curve);

  private:
    void _importNode(FbxNode& node);
    void _updateMaterials(FbxNode& node);
    void _importMesh(FbxMesh& mesh);
    void _readLayers(FbxMesh& mesh);
    void _readSkinData(FbxMesh& mesh);
    void _normalizeSkinWeights();
    uint16_t _getOrCreateBone(FbxCluster& cluster);
    void _distributePoints(FbxMesh& mesh);
    void _addPolygonIndices(int materialIndex, int polygonSize);
    void _addVertex(FbxMesh& mesh,
                    int materialIndex,
                    int globalIndex,
                    int polygonVertexIndex,
                    int polygonIndex,
                    int indexInPolygon);

  private:
    QDir _fileDir;

    using FBXMaterials = std::vector<FbxSurfaceMaterial*>;
    FBXMaterials _materials;

    using MeshDataSet = std::vector<CommonMeshGeometry>;
    MeshDataSet _meshData;
    FbxLayerElementNormal* _normalLayer;
    FbxLayerElementTangent* _tangentLayer;
    FbxLayerElementBinormal* _binormalLayer;
    FbxLayerElementUV* _texCoordLayer;
    FbxLayerElementMaterial* _materialLayer;

    Bones _bones;
    using BoneRef = CommonMeshGeometry::BoneRef;
    using SkeletonRef = CommonMeshGeometry::SkeletonRef;
    std::vector<SkeletonRef> _skeletonRefs;
  };

  inline bool operator == (
                        const BaseFbxImporter::MaterialOptions& left,
                        const BaseFbxImporter::MaterialOptions& right) noexcept
  {
    return  left.specularMapFromEmission == right.specularMapFromEmission &&
            left.metallicFromReflectionFactor ==
                                          right.metallicFromReflectionFactor &&
            left.blenderSpecular == right.blenderSpecular;
  }

  inline bool operator != (
                        const BaseFbxImporter::MaterialOptions& left,
                        const BaseFbxImporter::MaterialOptions& right) noexcept
  {
    return !(left == right);
  }

  inline bool operator < (
                        const BaseFbxImporter::MaterialOptions& left,
                        const BaseFbxImporter::MaterialOptions& right) noexcept
  {
    if(left.specularMapFromEmission < right.specularMapFromEmission)
    {
      return true;
    }

    if(left.specularMapFromEmission > right.specularMapFromEmission)
    {
      return false;
    }

    if(left.metallicFromReflectionFactor < right.metallicFromReflectionFactor)
    {
      return true;
    }

    if (left.metallicFromReflectionFactor > right.metallicFromReflectionFactor)
    {
      return false;
    }

    if(left.blenderSpecular < right.blenderSpecular)
    {
      return true;
    }

    return false;
  }

  inline bool operator > (
                        const BaseFbxImporter::MaterialOptions& left,
                        const BaseFbxImporter::MaterialOptions& right) noexcept
  {
    return right < left;
  }

  inline bool operator <= (
                        const BaseFbxImporter::MaterialOptions& left,
                        const BaseFbxImporter::MaterialOptions& right) noexcept
  {
    return !(left > right);
  }

  inline bool operator >= (
                        const BaseFbxImporter::MaterialOptions& left,
                        const BaseFbxImporter::MaterialOptions& right) noexcept
  {
    return !(left < right);
  }
}