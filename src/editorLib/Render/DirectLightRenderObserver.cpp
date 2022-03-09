#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/Objects/DirectLightObject.h>
#include <mtt/editorLib/Render/DirectLightRenderObserver.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/RenderScene.h>

#define ICON_FILE ":/editorLib/directLight.png"
#define ICON_SIZE 32

#define CAP_SEGMENTS 32
#define BODY_SEGMENTS 6

using namespace mtt;

DirectLightRenderObserver::DirectLightRenderObserver(
                                                    DirectLightObject& object,
                                                    CommonEditData& commonData,
                                                    RenderScene& renderScene) :
  AbstractLightRenderObserver(object, commonData, ICON_FILE, ICON_SIZE),
  _lightObject(object),
  _light(true, true, EditorApplication::instance().displayDevice()),
  _renderScene(renderScene)
{
  setLightObject(_light);

  connect(&_lightObject,
          &DirectLightObject::baseIlluminanceChanged,
          this,
          &DirectLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);
  connect(&_lightObject,
          &DirectLightObject::colorChanged,
          this,
          &DirectLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);
  _updateIlluminance();

  connect(&_lightObject,
          &DirectLightObject::distanceChanged,
          this,
          &DirectLightRenderObserver::_updateDistance,
          Qt::DirectConnection);
  _updateDistance();

  connect(&_lightObject,
          &DirectLightObject::radiusChanged,
          this,
          &DirectLightRenderObserver::_updateRadius,
          Qt::DirectConnection);
  _updateRadius();

  connect(&_lightObject,
          &DirectLightObject::shadowsEnabledChanged,
          this,
          &DirectLightRenderObserver::_updateShadowsEnabled,
          Qt::DirectConnection);
  _updateShadowsEnabled();

  connect(&_lightObject,
          &DirectLightObject::shadowmapSizeChanged,
          this,
          &DirectLightRenderObserver::_updateShadowMapSize,
          Qt::DirectConnection);
  _updateShadowMapSize();

  connect(&_lightObject,
          &DirectLightObject::cascadeSizeChanged,
          this,
          &DirectLightRenderObserver::_updateCascadeSize,
          Qt::DirectConnection);
  _updateCascadeSize();

  connect(&_lightObject,
          &DirectLightObject::blurSizeChanged,
          this,
          &DirectLightRenderObserver::_updateBlur,
          Qt::DirectConnection);
  _updateBlur();
}

void DirectLightRenderObserver::_updateIlluminance() noexcept
{
  _light.setIlluminance(_lightObject.baseIlluminance() * _lightObject.color());
}

void DirectLightRenderObserver::_updateDistance() noexcept
{
  _light.setDistance(_lightObject.distance());
  _updateDepthCamera();
  _updateCylinderMesh();
}

void DirectLightRenderObserver::_updateRadius() noexcept
{
  _light.setRadius(_lightObject.radius());
  _updateDepthCamera();
  _updateCylinderMesh();
}

void DirectLightRenderObserver::_updateCylinderMesh() noexcept
{
  if (_lightObject.radius() <= 0.f || _lightObject.distance() <= 0.f)
  {
    hullNode().resetGeometry();
    return;
  }

  try
  {
    hullNode().setCylinderGeometry( _lightObject.radius(),
                                    _lightObject.distance(),
                                    -_lightObject.distance() / 2.f,
                                    BODY_SEGMENTS,
                                    CAP_SEGMENTS);
  }
  catch (std::exception& error)
  {
    Log() << "DirectLightRenderObserver::_updateCylinderMesh: unable to update cylinder mesh: " << error.what();
  }
  catch (...)
  {
    Log() << "DirectLightRenderObserver::_updateCylinderMesh: unable to update cylinder mesh: unknown error.";
  }
}

void DirectLightRenderObserver::_updateDepthCamera() noexcept
{
  if (_shadowMapProvider == nullptr) return;
  float radius = _lightObject.radius();
  _shadowMapProvider->camera().setOrthoProjection(-radius,
                                                  radius,
                                                  -radius,
                                                  radius,
                                                  0,
                                                  _lightObject.distance());
}

void DirectLightRenderObserver::_updateShadowsEnabled() noexcept
{
  if(_lightObject.shadowsEnabled() && _shadowMapProvider == nullptr)
  {
    try
    {
      LogicalDevice& device = EditorApplication::instance().displayDevice();
      _shadowMapProvider.reset(new clPipeline::ShadowMapProvider(
                                                          2,
                                                          glm::uvec2(256, 256),
                                                          device));

      _light.setShadowMapProvider(_shadowMapProvider.get());
      positionRotateJoint().addChild(_shadowMapProvider->camera());
      _shadowMapProvider->setTargetField(&_renderScene.culledData());
      _updateDepthCamera();
      _updateShadowMapSize();
    }
    catch (std::exception& error)
    {
      _removeShadowmapProvider();
      Log() << "DirectLightRenderObserver::_updateShadowsEnabled: unable to update shadow map provider: " << error.what();
    }
    catch (...)
    {
      _removeShadowmapProvider();
      Log() << "DirectLightRenderObserver::_updateShadowsEnabled: unable to update shadow map provider: unknown error.";
    }
  }

  if(!_lightObject.shadowsEnabled() && _shadowMapProvider != nullptr)
  {
    _removeShadowmapProvider();
  }
}

void DirectLightRenderObserver::_removeShadowmapProvider() noexcept
{
  _light.setShadowMapProvider(nullptr);
  positionRotateJoint().removeChild(_shadowMapProvider->camera());
  _shadowMapProvider.reset();
}

void DirectLightRenderObserver::_updateShadowMapSize() noexcept
{
  if(_shadowMapProvider == nullptr) return;
  uint frameSize = std::max(1u, uint(_lightObject.shadowmapSize()));
  glm::uvec2 frameExtent(frameSize, frameSize);
  _shadowMapProvider->setFrameExtent(frameExtent);
}

void DirectLightRenderObserver::_updateCascadeSize() noexcept
{
  _light.setCascadeSize(_lightObject.cascadeSize());
}

void DirectLightRenderObserver::_updateBlur() noexcept
{
  _light.setBlurSize(_lightObject.blurSize());
}
