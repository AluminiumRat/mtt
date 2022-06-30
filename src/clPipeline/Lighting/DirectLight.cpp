#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace clPipeline;

DirectLight::DirectLight( bool forwardLightingEnabled,
                          bool defferedLightingEnabled,
                          LogicalDevice& device) :
  _device(device),
  _shadowmapProjectionMatrix(1.f),
  _shadowmapExtent(256),
  _shadowmapField(nullptr),
  _illuminance(1.f),
  _shadowDistance(50.f),
  _height(50.f),
  _direction(0.f, 0.f, -1.f),
  _cascadeSize(1),
  _blurSize(0.f)
{
  if(forwardLightingEnabled)
  {
    _forwardLightApplicator.reset(new DirectLightAreaModificator(*this));
    addChildProtected(*_forwardLightApplicator);
  }

  if(defferedLightingEnabled)
  {
    _defferedLightApplicator.reset(new DirectLightApplicator(*this, device));
    addChildProtected(*_defferedLightApplicator);
  }
}

void DirectLight::_updateShadowmapProjection() noexcept
{
  _shadowmapProjectionMatrix = CameraNode::projectionCorrect *
                                                  glm::ortho( -_shadowDistance,
                                                              _shadowDistance,
                                                              -_shadowDistance,
                                                              _shadowDistance,
                                                              0.f,
                                                              _height);

  if(_direction.z == 0.f) return;

  glm::vec3 slope = _direction / abs(_direction.z);
  glm::mat4 slopeMatrix = glm::mat4(glm::vec4(     1.f,      0.f, 0.f, 0.f),
                                    glm::vec4(     0.f,      1.f, 0.f, 0.f),
                                    glm::vec4( slope.x,  slope.y, 1.f, 0.f),
                                    glm::vec4(     0.f,      0.f, 0.f, 1.f));

  _shadowmapProjectionMatrix = _shadowmapProjectionMatrix * slopeMatrix;
}

void DirectLight::_resetPipelines() noexcept
{
  if (_defferedLightApplicator != nullptr)
  {
    _defferedLightApplicator->resetPipelines();
  }
  if (_forwardLightApplicator != nullptr) _forwardLightApplicator->reset();
}

void DirectLight::_resetShadowmapProvider() noexcept
{
  _resetPipelines();
  _shadowmapProvider.reset();
  _shadowmapSampler.reset();
}

void DirectLight::_updateShadowmapProvider()
{
  bool shadowsEnabled = _shadowmapExtent != 0 &&
                        _shadowDistance != 0 &&
                        _cascadeSize != 0 &&
                        _shadowmapField != nullptr;

  if (!shadowsEnabled)
  {
    if (_shadowmapProvider == nullptr) return;
    _resetShadowmapProvider();
  }
  else
  {
    if(_shadowmapProvider != nullptr) return;
    _resetPipelines();
    try
    {
      _shadowmapProvider.reset(new CascadeShadowMapProvider(
                                                  2,
                                                  glm::uvec2(_shadowmapExtent),
                                                  _device));
      _shadowmapProvider->setTargetField(_shadowmapField);

      _shadowmapSampler.reset(new Sampler(cascadeSize(),
                                          PipelineResource::VOLATILE,
                                          _device));

      for(size_t layerIndex = 0; layerIndex < cascadeSize(); layerIndex++)
      {
        _shadowmapSampler->setAttachedTexture(
                                          std::make_shared<Texture2D>(_device),
                                          layerIndex);
      }

      _shadowmapSampler->setMinFilter(VK_FILTER_LINEAR);
      _shadowmapSampler->setMagFilter(VK_FILTER_LINEAR);
      _shadowmapSampler->setMipmapMode(VK_SAMPLER_MIPMAP_MODE_NEAREST);
      _shadowmapSampler->setAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
      _shadowmapSampler->setAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
    }
    catch (...)
    {
      _resetShadowmapProvider();
      throw;
    }
  }
}

DirectLightData DirectLight::buildDrawData(
                              const DrawPlanBuildInfo& buildInfo) const noexcept
{
  glm::mat4 shiftedLocalToView =
        buildInfo.drawMatrices.localToViewMatrix * _getShiftMatrix(buildInfo);

  DirectLightData drawData;
  drawData.illuminance = illuminance();
  drawData.lightInverseDirection =
                                shiftedLocalToView * glm::vec4(-_direction, 0);
  drawData.lightInverseDirection =
                                glm::normalize(drawData.lightInverseDirection);
  drawData.height = height();
  drawData.shadowDistance = shadowDistance();
  drawData.clipToView = buildInfo.drawMatrices.clipToViewMatrix;
  drawData.viewToLocal = glm::inverse(shiftedLocalToView);
  drawData.localToShadowCoords =  glm::translate(glm::vec3(.5f, .5f, 0.f)) *
                                    glm::scale(glm::vec3(.5f, .5f, 1.f)) *
                                    _shadowmapProjectionMatrix;
  return drawData;
}

void DirectLight::updateShadowmapCamera(
                              CameraNode& camera,
                              const DrawPlanBuildInfo& buildInfo) const noexcept
{
  camera.setTransformMatrix(transformMatrix() * _getShiftMatrix(buildInfo));
  camera.setProjectionMatrix(_shadowmapProjectionMatrix);
}

glm::mat4 DirectLight::_getShiftMatrix(
                              const DrawPlanBuildInfo& buildInfo) const noexcept
{
  if(_shadowmapExtent == 0) return glm::mat4(1.f);

  glm::mat4 viewToLocalMatrix =
                        glm::inverse(buildInfo.drawMatrices.localToViewMatrix);
  glm::vec3 viewCenter = viewToLocalMatrix *
                        glm::vec4(buildInfo.currentViewInfo.viewPosition, 1.f);

  glm::vec3 slope = _direction / abs(_direction.z);
  glm::vec3 cameraPosition( viewCenter.x + slope.x * viewCenter.z,
                            viewCenter.y + slope.y * viewCenter.z,
                            0.f);

  float texelSize = 2.f * _shadowDistance / _shadowmapExtent;
  cameraPosition = glm::round(cameraPosition / texelSize) * texelSize;

  return glm::translate(cameraPosition);
}

size_t DirectLight::unculledDrawablesNumber() const noexcept
{
  return _defferedLightApplicator == nullptr ? 0 : 1;
}

Drawable& DirectLight::unculledDrawable(size_t index) noexcept
{
  if (_defferedLightApplicator == nullptr) Abort("DirectLight::culledDrawable: no culled drawables available.");
  else return *_defferedLightApplicator;
}

size_t DirectLight::areaModificatorsNumber() const noexcept
{
  return _forwardLightApplicator == nullptr ? 0 : 1;
}

AreaModificator& DirectLight::areaModificator(size_t index) noexcept
{
  if (_forwardLightApplicator == nullptr) Abort("DirectLight::areaModificator: no area modificators available.");
  else return *_forwardLightApplicator;
}
