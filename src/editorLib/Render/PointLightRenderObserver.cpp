#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/AsyncTasks/UploadTextureTask.h>
#include <mtt/editorLib/Objects/PointLightObject.h>
#include <mtt/editorLib/Render/PointLightRenderObserver.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/RenderScene.h>

#define ICON_FILE ":/editorLib/pointLight.png"
#define ICON_SIZE 32

#define SPHERE_SEGMENTS 32

using namespace mtt;

PointLightRenderObserver::PointLightRenderObserver( PointLightObject& object,
                                                  CommonEditData& commonData,
                                                  RenderScene& renderScene) :
  AbstractLightRenderObserver(object, commonData, ICON_FILE, ICON_SIZE),
  _lightObject(object),
  _light(true, true, EditorApplication::instance().displayDevice()),
  _cubemapObserver(object.filterCubemap()),
  _renderScene(renderScene)
{
  setLightObject(_light);

  connect(&_lightObject,
          &PointLightObject::baseIlluminanceChanged,
          this,
          &PointLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);
  connect(&_lightObject,
          &PointLightObject::colorChanged,
          this,
          &PointLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);
  _updateIlluminance();

  connect(&_lightObject,
          &PointLightObject::distanceChanged,
          this,
          &PointLightRenderObserver::_updateDistance,
          Qt::DirectConnection);
  _updateDistance();

  connect(&_lightObject,
          &PointLightObject::shadowsEnabledChanged,
          this,
          &PointLightRenderObserver::_updateShadowsEnabled,
          Qt::DirectConnection);
  _updateShadowsEnabled();

  connect(&_lightObject,
          &PointLightObject::shadowmapSizeChanged,
          this,
          &PointLightRenderObserver::_updateShadowMapSize,
          Qt::DirectConnection);
  _updateShadowMapSize();

  connect(&_lightObject,
          &PointLightObject::shadowBlurChanged,
          this,
          &PointLightRenderObserver::_updateBlur,
          Qt::DirectConnection);
  _updateBlur();

  _cubemapObserver.setCallback(
    [&](std::shared_ptr<CubeTexture> texture)
    {
      _light.setFilterTexture(texture);
    });
}

void PointLightRenderObserver::_updateIlluminance() noexcept
{
  _light.setIlluminance(_lightObject.baseIlluminance() * _lightObject.color());
}

void PointLightRenderObserver::_updateDistance() noexcept
{
  _light.setDistance(_lightObject.distance());
  _updateSphereMesh();
}

void PointLightRenderObserver::_updateSphereMesh() noexcept
{
  if (_lightObject.distance() <= 0.f)
  {
    hullNode().resetGeometry();
    return;
  }

  try
  {
    hullNode().setSphereGeometry( _lightObject.distance(), SPHERE_SEGMENTS);
  }
  catch (std::exception& error)
  {
    Log() << "PointLightRenderObserver::_updateConeMesh: unable to update mesh: " << error.what();
  }
  catch (...)
  {
    Log() << "PointLightRenderObserver::_updateConeMesh: unable to update mesh: unknown error.";
  }
}

void PointLightRenderObserver::_updateShadowsEnabled() noexcept
{
  if(_lightObject.shadowsEnabled() && _shadowMapProvider == nullptr)
  {
    try
    {
      LogicalDevice& device = EditorApplication::instance().displayDevice();
      _shadowMapProvider.reset(new clPipeline::CubeShadowmapProvider( 2,
                                                                      256,
                                                                      device));

      _light.setShadowmapProvider(_shadowMapProvider.get());
      _shadowMapProvider->setTargetField(&_renderScene.culledData());
      _updateShadowMapSize();
    }
    catch (std::exception& error)
    {
      _removeShadowmapProvider();
      Log() << "PointLightRenderObserver::_updateShadowsEnabled: unable to update shadow map provider: " << error.what();
    }
    catch (...)
    {
      _removeShadowmapProvider();
      Log() << "PointLightRenderObserver::_updateShadowsEnabled: unable to update shadow map provider: unknown error.";
    }
  }

  if(!_lightObject.shadowsEnabled() && _shadowMapProvider != nullptr)
  {
    _removeShadowmapProvider();
  }
}

void PointLightRenderObserver::_removeShadowmapProvider() noexcept
{
  _light.setShadowmapProvider(nullptr);
  _shadowMapProvider.reset();
}

void PointLightRenderObserver::_updateShadowMapSize() noexcept
{
  if (_shadowMapProvider == nullptr) return;
  uint32_t frameSize = std::max(1u, uint32_t(_lightObject.shadowmapSize()));
  _shadowMapProvider->setFrameExtent(frameSize);
}

void PointLightRenderObserver::_updateBlur() noexcept
{
  _light.setBlurAngle(_lightObject.shadowBlur());
}
