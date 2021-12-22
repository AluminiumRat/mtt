#pragma once

#include <optional>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <mtt/Utilities/Box.h>

namespace mtt
{
  struct CommonMeshGeometry
  {
    static const size_t MAX_BONES_PER_VERTEX = 4;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> binormals;
    std::vector<glm::vec2> texCoords;

    struct BoneRef
    {
      uint8_t boneIndex = 0;
      float weight = 0;
    };
    struct SkeletonRef
    {
      uint16_t bonesNumber = 0;
      BoneRef bones[MAX_BONES_PER_VERTEX];
    };
    std::vector<SkeletonRef> skeletonRefs;

    std::vector<uint16_t> triangleIndices;
    std::vector<uint16_t> lineIndices;

    CommonMeshGeometry() = default;
    CommonMeshGeometry(const CommonMeshGeometry&) = default;
    CommonMeshGeometry& operator = (const CommonMeshGeometry&) = default;
    CommonMeshGeometry(CommonMeshGeometry&&) = default;
    CommonMeshGeometry& operator = (CommonMeshGeometry&&) = default;
    ~CommonMeshGeometry() = default;

    Box calculateBoundingBox() const noexcept;
  };
}