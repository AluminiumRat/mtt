#include <stdexcept>

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
  positionRotateJoint().addChild(_light);

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
          &PointLightObject::fadeTypeChanged,
          this,
          &PointLightRenderObserver::_updateFade,
          Qt::DirectConnection);
  _updateFade();

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

void PointLightRenderObserver::_updateFade() noexcept
{
  _light.setFadeType(_lightObject.fadeType());
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
  try
  {
    if (_lightObject.shadowsEnabled())
    {
      _light.setShadowmapField(&_renderScene.culledData());
    }
    else _light.setShadowmapField(nullptr);
  }
  catch (std::exception& error)
  {
    Log() << "PointLightRenderObserver::_updateShadowsEnabled: " << error.what();
  }
  catch (...)
  {
    Log() << "PointLightRenderObserver::_updateShadowsEnabled: unknown error.";
  }
}

void PointLightRenderObserver::_updateShadowMapSize() noexcept
{
  try
  {
    unsigned int frameSize = std::max(
                                    1u,
                                    unsigned int(_lightObject.shadowmapSize()));
    _light.setShadowmapExtent(frameSize);
  }
  catch (std::exception& error)
  {
    Log() << "PointLightRenderObserver::_updateShadowMapSize: " << error.what();
  }
  catch (...)
  {
    Log() << "PointLightRenderObserver::_updateShadowMapSize: unknown error.";
  }
}

void PointLightRenderObserver::_updateBlur() noexcept
{
  _light.setBlurAngle(_lightObject.shadowBlur());
}
