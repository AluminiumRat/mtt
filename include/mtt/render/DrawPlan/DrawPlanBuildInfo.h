#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <mtt/render/DrawPlan/DrawPlanBuildExtraData.h>
#include <mtt/render/RenderPass/AbstractRenderPass.h>
#include <mtt/render/SceneRenderer/AbstractFrame.h>
#include <mtt/render/SceneRenderer/AbstractFrameBuilder.h>
#include <mtt/utilities/UID.h>
#include <mtt/utilities/ViewFrustum.h>

namespace mtt
{
  class AbstractFramePlan;
  class AreaModificatorSet;
  class CameraNode;
  class DrawPlan;

  struct DrawMatrices
  {
    inline static const char bindingName[] = "drawMatricesBinding";

    alignas(16) glm::mat4 localToViewMatrix = glm::mat4(1);
    alignas(16) glm::mat3x4 normalsLocalToViewMatrix = glm::mat4(1);
    alignas(16) glm::mat4 projectionMatrix = glm::mat4(1);
    alignas(16) glm::mat4 clipToViewMatrix = glm::mat4(1);
    alignas(16) glm::mat4 viewToReflection = glm::mat4(1);
  };

  struct ScreenSizeInfo
  {
    glm::vec2 nearPlaneSize = glm::vec2(1);   /// Size of screen at near clip
                                              /// distance in scene units in
                                              /// view space

    float nearMppx = 0;                       /// Meter per pixel ratio at near
                                              /// clip distance.

    glm::vec2 farPlaneSize = glm::vec2(1);    /// Size of screen at far clip
                                              /// distance in scene units in
                                              /// view space

    float farMppx = 0;                        /// Meter per pixel ratio at far
                                              /// clip distance.
  };

  /// mppx = frontDistance * slope + intercept
  struct MppxDistanceFunction
  {
    alignas(4) float intercept;
    alignas(4) float slope;

    inline float mppx(float distance) const noexcept;
  };

  /// All info in current view space
  struct ViewInfo
  {
    glm::vec3 viewPosition = glm::vec3(0);
    glm::vec3 viewFront = glm::vec3(0, 0, -1);
    glm::vec3 viewUp = glm::vec3(0, 1, 0);
    glm::vec3 viewRight = glm::vec3(0, 1, 0);

    ScreenSizeInfo screenSizeInfo;
    float nearCameraDistance = 1;
    float farCameraDistance = 1;

    /// Distance from viewpoint to "point".
    inline float distanceToPoint(const glm::vec3& point) const noexcept;

    /// Distance from viewpoint to "point" along the viewFront vector.
    inline float frontDistanceToPoint(const glm::vec3& point) const noexcept;

    /// Distance from viewpoint to "point" along the viewFront vector.
    /// Distance normalized to farCameraDistance
    inline float normalizedDistance(const glm::vec3& point) const noexcept;
    inline glm::vec2 screenSize(const glm::vec3& point) const noexcept;

    ///Meter per pixel ratio
    inline float mppx(const glm::vec3& point) const noexcept;

    inline MppxDistanceFunction mppxFunction() const noexcept;
  };

  struct DrawPlanBuildInfo
  {
    DrawPlan& drawPlan;

    AbstractFramePlan* currentFramePlan;
    FrameType frameType;
    AbstractFrameBuilder* builder;
    AbstractFrame* frame;

    VkViewport viewport;
    VkRect2D scissor;

    DrawMatrices drawMatrices;

    ViewInfo rootViewInfo;        /// View info for root frame of drawing
    ViewInfo currentViewInfo;     /// View info for current frame

    ViewFrustum viewFrustum;      /// Frustum for current frame in view space

    const AreaModificatorSet* areaModificators;

    DrawPlanBuildExtraData extraData;

    explicit DrawPlanBuildInfo(DrawPlan& theDrawPlan);
    DrawPlanBuildInfo(const DrawPlanBuildInfo&) = delete;
    DrawPlanBuildInfo& operator = (const DrawPlanBuildInfo&) = delete;
    ~DrawPlanBuildInfo() noexcept = default;

    /// If rootViewInfo is nullptr then new rootViewInfo will be generated from
    /// camera
    void adjustFrameRender( const VkViewport& newViewport,
                            const VkRect2D& newScissor,
                            AbstractFramePlan& theFramePlan,
                            const CameraNode& camera,
                            ViewInfo* newRootViewInfo = nullptr) noexcept;

    /// Translate point from local coordinate system to view space
    inline glm::vec3 viewSpacePoint(const glm::vec3& point) const noexcept;

    /// Distance from "point" to viewpoint.
    /// Warning!!! Returns the distance in view space (without scale), but 
    /// "point" is in the local coordinate system(no drawing matrices are
    /// applied).
    /// Can be used to calculate render priority.
    inline float distanceToPoint(const glm::vec3& point) const noexcept;

    /// Distance from "point" to viewpoint.
    /// Distance normalized to farCameraDistance.
    /// "point" is in the local coordinate system(no drawing matrices are
    /// applied).
    /// Can be used to calculate render priority.
    inline float normalizedDistance(const glm::vec3& point) const noexcept;

    /// Calculate render priority to order "Near objects will be rendered first"
    /// "point" is in the local coordinate system(no drawing matrices are
    /// applied).
    inline float getPriorityNearFirstOrder(
                                        const glm::vec3& point) const noexcept;
    /// Calculate render priority to order "Far objects will be rendered first"
    /// "point" is in the local coordinate system(no drawing matrices are
    /// applied).
    inline float getPriorityFarFirstOrder(
                                        const glm::vec3& point) const noexcept;
  };

  inline float MppxDistanceFunction::mppx(float distance) const noexcept
  {
    return distance * slope + intercept;
  }

  inline float ViewInfo::distanceToPoint(const glm::vec3& point) const noexcept
  {
    return glm::length(point - viewPosition);
  }

  inline float ViewInfo::frontDistanceToPoint(
                                        const glm::vec3& point) const noexcept
  {
    glm::vec3 toPoint = point - viewPosition;
    return glm::dot(viewFront, toPoint);
  }

  inline float ViewInfo::normalizedDistance(
                                          const glm::vec3& point) const noexcept
  {
    float distance = frontDistanceToPoint(point);
    distance = glm::clamp(distance, nearCameraDistance, farCameraDistance);

    return glm::clamp(distance / farCameraDistance, 0.f, 1.f);
  }

  inline glm::vec2 ViewInfo::screenSize(const glm::vec3& point) const noexcept
  {
    if(nearCameraDistance == farCameraDistance)
                                            return screenSizeInfo.nearPlaneSize;

    float distance = frontDistanceToPoint(point);
    distance = glm::clamp(distance, nearCameraDistance, farCameraDistance);

    distance = glm::clamp(distance, nearCameraDistance, farCameraDistance);
    float rate = (distance - nearCameraDistance) /
                                      (farCameraDistance - nearCameraDistance);
    return glm::mix(screenSizeInfo.nearPlaneSize,
                    screenSizeInfo.farPlaneSize,
                    rate);
  }

  inline float ViewInfo::mppx(const glm::vec3& point) const noexcept
  {
    if (nearCameraDistance == farCameraDistance) return screenSizeInfo.nearMppx;

    float distance = frontDistanceToPoint(point);
    distance = glm::clamp(distance, nearCameraDistance, farCameraDistance);

    float rate = (distance - nearCameraDistance) /
                                      (farCameraDistance - nearCameraDistance);

    return glm::mix(screenSizeInfo.nearMppx, screenSizeInfo.farMppx, rate);
  }

  inline MppxDistanceFunction ViewInfo::mppxFunction() const noexcept
  {
    MppxDistanceFunction result;
    if (nearCameraDistance == farCameraDistance)
    {
      result.intercept = screenSizeInfo.nearMppx;
      result.slope = 0.f;
    }
    else
    {
      result.slope = (screenSizeInfo.farMppx - screenSizeInfo.nearMppx) /
                                      (farCameraDistance - nearCameraDistance);
      result.intercept =
                    screenSizeInfo.nearMppx - result.slope * nearCameraDistance;
    }
    return result;
  }

  inline glm::vec3 DrawPlanBuildInfo::viewSpacePoint(
                                          const glm::vec3& point) const noexcept
  {
    return drawMatrices.localToViewMatrix * glm::vec4(point, 1);
  }

  inline float DrawPlanBuildInfo::distanceToPoint(
                                          const glm::vec3& point) const noexcept
  {
    return currentViewInfo.distanceToPoint(viewSpacePoint(point));
  }

  inline float DrawPlanBuildInfo::normalizedDistance(
                                          const glm::vec3& point) const noexcept
  {
    return currentViewInfo.normalizedDistance(viewSpacePoint(point));
  }

  inline float DrawPlanBuildInfo::getPriorityNearFirstOrder(
                                          const glm::vec3& point) const noexcept
  {
    float distance = normalizedDistance(point);
    return 2.f - distance;
  }

  inline float DrawPlanBuildInfo::getPriorityFarFirstOrder(
                                          const glm::vec3& point) const noexcept
  {
    float distance = normalizedDistance(point);
    return distance;
  }
}