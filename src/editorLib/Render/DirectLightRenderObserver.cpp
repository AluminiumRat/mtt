#include <stdexcept>

#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/Objects/DirectLightObject.h>
#include <mtt/editorLib/Render/DirectLightRenderObserver.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/RenderScene.h>
#include <mtt/utilities/Log.h>

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
  positionJoint().addChild(_light);

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
          &DirectLightObject::rotationChanged,
          this,
          &DirectLightRenderObserver::_updateDirection,
          Qt::DirectConnection);
  _updateDirection();

  connect(&_lightObject,
          &DirectLightObject::shadowDistanceChanged,
          this,
          &DirectLightRenderObserver::_updateShadowDistance,
          Qt::DirectConnection);
  _updateShadowDistance();

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
          &DirectLightObject::shadowBlurChanged,
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
  _light.setHeight(_lightObject.distance());
}

void DirectLightRenderObserver::_updateDirection() noexcept
{
  glm::mat4 transform = _lightObject.localToWorldTransform();
  glm::vec3 newDirection = transform * glm::vec4(0.f, 0.f, -1.f, 0.f);
  _light.setDirection(newDirection);
}

void DirectLightRenderObserver::_updateShadowDistance() noexcept
{
  try
  {
    _light.setShadowDistance(_lightObject.shadowDistance());
  }
  catch (std::exception& error)
  {
    Log() << "DirectLightRenderObserver::_updateRadius: " << error.what();
  }
  catch (...)
  {
    Log() << "DirectLightRenderObserver::_updateRadius: unknown error.";
  }
}

void DirectLightRenderObserver::_updateShadowsEnabled() noexcept
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
    Log() << "DirectLightRenderObserver::_updateShadowsEnabled: " << error.what();
  }
  catch (...)
  {
    Log() << "DirectLightRenderObserver::_updateShadowsEnabled: unknown error.";
  }
}

void DirectLightRenderObserver::_updateShadowMapSize() noexcept
{
  unsigned int frameSize = std::max(1u,
                                    unsigned int(_lightObject.shadowmapSize()));
  try
  {
    _light.setShadowmapExtent(frameSize);
  }
  catch (std::exception& error)
  {
    Log() << "DirectLightRenderObserver::_updateShadowMapSize: " << error.what();
  }
  catch (...)
  {
    Log() << "DirectLightRenderObserver::_updateShadowMapSize: unknown error.";
  }
}

void DirectLightRenderObserver::_updateCascadeSize() noexcept
{
  try
  {
    _light.setCascadeSize(_lightObject.cascadeSize());
  }
  catch (std::exception& error)
  {
    Log() << "DirectLightRenderObserver::_updateCascadeSize: " << error.what();
  }
  catch (...)
  {
    Log() << "DirectLightRenderObserver::_updateCascadeSize: unknown error.";
  }
}

void DirectLightRenderObserver::_updateBlur() noexcept
{
  _light.setBlurSize(_lightObject.shadowBlur());
}
