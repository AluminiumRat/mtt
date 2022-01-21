#pragma once

#include <glm/vec2.hpp>

#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/AbstractNode.h>
#include <mtt/utilities/ViewFrustum.h>

namespace mtt
{
  class CameraNode : public AbstractNode
  {
  public:
    CameraNode() noexcept;
    CameraNode(const CameraNode&) = delete;
    CameraNode& operator = (const CameraNode&) = delete;
    virtual ~CameraNode() noexcept = default;

    virtual void setTransformMatrix(
                                  const glm::mat4& newValue) noexcept override;

    inline const glm::vec3& eyePoint() const noexcept;
    inline const glm::vec3& frontVector() const noexcept;
    inline const glm::vec3& upVector() const noexcept;
    inline const glm::vec3& rightVector() const noexcept;
    inline const glm::mat4 viewMatrix() const noexcept;

    void setProjectionMatrix(glm::mat4 newValue) noexcept;
    void setPerspectiveProjection(float fovY,
                                  float aspect,
                                  float zNear,
                                  float zFar) noexcept;
    void setOrthoProjection(float left,
                            float right,
                            float bottom,
                            float top,
                            float zNear,
                            float zFar) noexcept;
    inline const glm::mat4 projectionMatrix() const noexcept;
    inline const glm::mat4 inverseProjectionMatrix() const noexcept;
    inline float nearDistance() const noexcept;
    inline float farDistance() const noexcept;

    /// Frustum in view space
    inline const ViewFrustum& frustum() const noexcept;

    DrawMatrices buildDrawMatrices() const noexcept;

    ViewInfo buildViewInfo(const glm::uvec2 screenExtent) const noexcept;
    ScreenSizeInfo buildScreenSizeInfo(
                                  const glm::uvec2 screenExtent) const noexcept;

    /// Translate root view info from the parent view system to view the
    /// view system of this camera.
    void applyRootViewInfo( ViewInfo& rootInfo,
                            const glm::mat4& localToParentView) const noexcept;

    /// Returns direction from eyePoint
    glm::vec3 getDirection(const glm::vec2& screenCoordinates) const noexcept;

  private:
    void _updateFromPositionMatrix() noexcept;
    void _updateFromProjectionMatrix() noexcept;

  private:
    glm::mat4 _viewMatrix;
    glm::vec3 _eyePoint;
    glm::vec3 _frontVector;
    glm::vec3 _upVector;
    glm::vec3 _rightVector;

    glm::mat4 _projectionMatrix;
    glm::mat4 _inverseProjectionMatrix;

    ViewFrustum _frustum;

    float _nearDistance;
    float _farDistance;
  };

  inline const glm::vec3& CameraNode::eyePoint() const noexcept
  {
    return _eyePoint;
  }

  inline const glm::vec3& CameraNode::frontVector() const noexcept
  {
    return _frontVector;
  }

  inline const glm::vec3& CameraNode::upVector() const noexcept
  {
    return _upVector;
  }

  inline const glm::vec3& CameraNode::rightVector() const noexcept
  {
    return _rightVector;
  }

  inline const glm::mat4 CameraNode::viewMatrix() const noexcept
  {
    return _viewMatrix;
  }

  inline const glm::mat4 CameraNode::projectionMatrix() const noexcept
  {
    return _projectionMatrix;
  }

  inline const glm::mat4 CameraNode::inverseProjectionMatrix() const noexcept
  {
    return _inverseProjectionMatrix;
  }

  inline float CameraNode::nearDistance() const noexcept
  {
    return _nearDistance;
  }

  inline float CameraNode::farDistance() const noexcept
  {
    return _farDistance;
  }

  inline const ViewFrustum& CameraNode::frustum() const noexcept
  {
    return _frustum;
  }
}