#pragma once

#include <map>
#include <memory>
#include <vector>

#include <mtt/Fbx/BaseFbxImporter.h>

#include <Objects/MeshObject.h>
#include <Objects/SkeletonObject.h>

class FbxDrawableImporter : public mtt::BaseFbxImporter
{
public:
  struct Result
  {
    std::vector<std::unique_ptr<SkeletonObject>> skeleton;
    std::vector<std::unique_ptr<MeshObject>> drawables;

    Result() = default;
    Result(const Result&) = delete;
    Result& operator = (const Result&) = delete;
    Result(Result&&) = default;
    Result& operator = (Result&&) = default;
    ~Result() = default;
  };

public:
  FbxDrawableImporter() = default;
  FbxDrawableImporter(const FbxDrawableImporter&) = delete;
  FbxDrawableImporter& operator = (const FbxDrawableImporter&) = delete;
  virtual ~FbxDrawableImporter() noexcept = default;
  
  Result import(const char* filename,
                const MaterialOptions& materialOptions);

protected:
  virtual void pushTranslation(FbxNode& node) override;
  virtual void popTranslation() override;
  virtual void processMesh( mtt::CommonMeshGeometry&& vertices,
                            const FbxSurfaceMaterial& material,
                            const Bones& bones,
                            const FbxMesh& mesh) override;

private:
  void _resolveBoneLinks();
  void _importMaterial( MeshObject& mesh,
                        const FbxSurfaceMaterial& material);

private:
  MaterialOptions _materialOptions;

  Result _result;
  std::vector<SkeletonObject*> _skeletonStack;

  using SkeletonMap = std::map<FbxNode*, SkeletonObject*>;
  SkeletonMap _skeletonMap;

  struct BonesLink
  {
    MeshObject* mesh;
    Bones bones;
  };
  std::vector<BonesLink> _boneLinks;
};