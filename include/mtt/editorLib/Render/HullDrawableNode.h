#pragma once

#include <vector>

#include <glm/vec3.hpp>

#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

namespace mtt
{
  class HullDrawableNode : public SimpleDrawableNode
  {
  public:
    HullDrawableNode();
    HullDrawableNode(const HullDrawableNode&) = delete;
    HullDrawableNode& operator = (const HullDrawableNode&) = delete;
    virtual ~HullDrawableNode() noexcept = default;

    void setColor(const glm::vec3& color);

    /// newGeometry should be in VK_PRIMITIVE_TOPOLOGY_LINE_LIST topology
    void setGeometry(const std::vector<glm::vec3> newGeometry);
    void resetGeometry() noexcept;

    void setSphereGeometry(float radius, unsigned int segmentNumber);
    /// Create cylinder along the Z axys centered at the zero point.
    /// center shift along the z axis.
    void setCylinderGeometry( float radius,
                              float length,
                              float centerShift,
                              unsigned int bodySegmentNumber,
                              unsigned int capSegmentNumber);
    /// Create circle in XY plane.
    void setCircleGeometry(float radius, unsigned int segmentNumber);
    void setBoxGeometry(glm::vec3 size);

  private:
    Mesh _mesh;
  };
}