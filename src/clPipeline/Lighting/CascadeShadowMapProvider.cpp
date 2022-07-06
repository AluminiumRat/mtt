#include <glm/gtx/transform.hpp>

#include <mtt/clPipeline/Lighting/CascadeShadowMapProvider.h>
#include <mtt/render/DrawPlan/DrawPlan.h>
#include <mtt/render/SceneGraph/DrawVisitor.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/clip.h>

using namespace mtt;
using namespace mtt::clPipeline;

CascadeShadowMapProvider::CascadeShadowMapProvider(
                                                size_t framePoolsNumber,
                                                glm::uvec2 opaqueMapExtent,
                                                glm::uvec2 transparentMapExtent,
                                                LogicalDevice& device) :
  ShadowMapProvider(framePoolsNumber,
                    opaqueMapExtent,
                    transparentMapExtent,
                    device)
{
}

CascadeShadowMapProvider::Area CascadeShadowMapProvider::_getTopArea(
                                    const CameraNode& shadowmapCamera,
                                    const CameraNode& viewCamera) const noexcept
{
  glm::mat4 clipToView = viewCamera.inverseProjectionMatrix();
  glm::mat4 toLightView = shadowmapCamera.viewMatrix() *
                                                  viewCamera.transformMatrix();
  glm::mat4 toLightClip = shadowmapCamera.projectionMatrix();
  glm::mat4 pointsTransform = toLightClip * toLightView * clipToView;

  glm::vec4 frustumPoints[] = { pointsTransform * glm::vec4(-1, -1, 1, 1),
                                pointsTransform * glm::vec4( 1, -1, 1, 1),
                                pointsTransform * glm::vec4( 1,  1, 1, 1),
                                pointsTransform * glm::vec4(-1,  1, 1, 1),
                                pointsTransform * glm::vec4(-1, -1, 0, 1),
                                pointsTransform * glm::vec4( 1, -1, 0, 1),
                                pointsTransform * glm::vec4( 1,  1, 0, 1),
                                pointsTransform * glm::vec4(-1,  1, 0, 1)};
  glm::vec4 clipArea{1,1,-1,-1};

  for(glm::vec4& point : frustumPoints)
  {
    if(point.x > point.w) point.x = point.w;
    if(point.x < -point.w) point.x = -point.w;
    point.x /= point.w;

    if(point.y > point.w) point.y = point.w;
    if(point.y < -point.w) point.y = -point.w;
    point.y /= point.w;

    clipArea.x = std::min(clipArea.x, point.x);
    clipArea.y = std::min(clipArea.y, point.y);
    clipArea.z = std::max(clipArea.z, point.x);
    clipArea.w = std::max(clipArea.w, point.y);
  }

  Area result;
  result.topleftCorner = glm::vec2(clipArea.x, clipArea.y);
  result.size = glm::vec2(clipArea.z - clipArea.x, clipArea.w - clipArea.y);
  return result;
}

glm::vec2 CascadeShadowMapProvider::_getCascadeDirectionPoint(
                                          const CameraNode& shadowmapCamera,
                                          const CameraNode& viewCamera,
                                          glm::vec2 startPoint) const noexcept
{
  glm::mat4 toLightView = shadowmapCamera.viewMatrix() *
                                                  viewCamera.transformMatrix();
  glm::mat4 toLightClip = shadowmapCamera.projectionMatrix();
  glm::mat4 endPointTransform = toLightClip * toLightView;
  glm::vec4 endPoint = endPointTransform * glm::vec4(0.f, 0.f, 0.f, 1.f);
  glm::vec4 startPoint4d(startPoint, .5f, 1.f);
  clipSegment(startPoint4d, endPoint);
  glm::vec2 directionPoint = endPoint / endPoint.w;

  return directionPoint;
}

CascadeShadowMapProvider::Area CascadeShadowMapProvider::_alignArea(
                                              const Area& source) const noexcept
{
  float areaSize = std::max(source.size.x, source.size.y);
  float sizeOrder = ceil(log2(areaSize));
  areaSize = exp2(sizeOrder);
  if(areaSize > 2.f) areaSize = 2.f;

  float granularity = areaSize / opaqueMapExtent().x * 2.f;

  glm::vec2 sourceCenter = source.topleftCorner + source.size / 2.f;
  glm::vec2 alignedCorner = sourceCenter - glm::vec2(areaSize) / 2.f;
  alignedCorner = glm::round(alignedCorner / granularity) * granularity;
  alignedCorner = glm::clamp(alignedCorner, -1.f, 1.f);

  CascadeShadowMapProvider::Area alignedArea;
  alignedArea.topleftCorner = alignedCorner;
  alignedArea.size = glm::vec2(areaSize);

  return alignedArea;
}

CascadeShadowMapProvider::CascadeInfo
                    CascadeShadowMapProvider::getShadowMaps(
                                              const CameraNode& shadowmapCamera,
                                              const CameraNode& viewCamera,
                                              size_t cascadeSize,
                                              DrawPlanBuildInfo& buildInfo)
{
  if(cascadeSize == 0) return CascadeInfo{};

  CascadeShadowMapProvider::Area area =
                                      _getTopArea(shadowmapCamera, viewCamera);
  if(area.size.x <= 0 || area.size.y <= 0) return CascadeInfo();

  glm::vec2 centralPoint = area.topleftCorner + area.size / 2.f;
  glm::vec2 directionPoint = _getCascadeDirectionPoint( shadowmapCamera,
                                                        viewCamera,
                                                        centralPoint);
  glm::vec2 shift = directionPoint - centralPoint;

  CascadeInfo result;
  result.reserve(cascadeSize);

  for(size_t layerIndex = 0; layerIndex < cascadeSize; layerIndex++)
  {
    Area alignedArea = _alignArea(area);

    CameraNode renderCamera;
    _setupRenderCamera(shadowmapCamera, renderCamera, alignedArea);

    ShadowMapsInfo info{};
    info.maps = ShadowMapProvider::getShadowMaps(renderCamera, buildInfo);

    glm::vec2 uvAreaSize = alignedArea.size / 2.f;
    glm::vec2 uvAreaCorner =
              alignedArea.topleftCorner * glm::vec2(.5f, .5f) + glm::vec2(.5f);

    info.coordCorrection.multiplicator = 1.f / uvAreaSize.x;
    info.coordCorrection.shift = glm::vec2( -uvAreaCorner.x / uvAreaSize.x,
                                            -uvAreaCorner.y / uvAreaSize.y);
    result.push_back(info);

    shift /= 2.f;

    glm::vec2 center = area.topleftCorner + area.size / 2.f;
    center = center + shift;
    area.size /= 2.f;
    area.topleftCorner = center - area.size / 2.f;
  }

  return result;
}

void CascadeShadowMapProvider::_setupRenderCamera(
                                            const CameraNode& topViewCamera,
                                            CameraNode& renderCamera,
                                            const Area& mapPart) const noexcept
{
  renderCamera.setTransformMatrix(topViewCamera.transformMatrix());

  glm::mat4 projectionMatrix = topViewCamera.projectionMatrix();

  glm::vec2 areaCenter = mapPart.topleftCorner + mapPart.size / 2.f;
  glm::vec3 projectionShift(-areaCenter, 0.f);
  projectionMatrix = glm::translate(projectionShift) * projectionMatrix;

  glm::vec3 projectionScale(2.f / mapPart.size.x,
                            2.f / mapPart.size.y,
                            1.f);
  projectionMatrix = glm::scale(projectionScale) * projectionMatrix;

  renderCamera.setProjectionMatrix(projectionMatrix);
}
