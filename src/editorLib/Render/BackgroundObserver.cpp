#include <stdexcept>

#include <mtt/application/Application.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Render/BackgroundObserver.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

BackgroundObserver::BackgroundObserver( BackgroundObject& object,
                                        CommonEditData& commonData) :
  Object3DRenderObserver(object, commonData),
  _object(object),
  _backgroundRenderer(Application::instance().displayDevice()),
  _cubemapObserver(object.cubemap())
{
  connect(&_object,
          &BackgroundObject::visibleChanged,
          this,
          &BackgroundObserver::_updateLight,
          Qt::DirectConnection);
  connect(&_object,
          &BackgroundObject::lightEnabledChanged,
          this,
          &BackgroundObserver::_updateLight,
          Qt::DirectConnection);
  _updateLight();

  connect(&_object,
          &BackgroundObject::luminanceChanged,
          this,
          &BackgroundObserver::_updateLuminance,
          Qt::DirectConnection);
  connect(&_object,
          &BackgroundObject::colorChanged,
          this,
          &BackgroundObserver::_updateLuminance,
          Qt::DirectConnection);
  _updateLuminance();

  connect(&_object,
          &BackgroundObject::dissolutionStartDistanceChanged,
          this,
          &BackgroundObserver::_updateDissolutionStartDistance,
          Qt::DirectConnection);
  _updateDissolutionStartDistance();

  connect(&_object,
          &BackgroundObject::dissolutionLengthChanged,
          this,
          &BackgroundObserver::_updateDissolutionLength,
          Qt::DirectConnection);
  _updateDissolutionLength();

  _cubemapObserver.setCallback(
    [&](std::shared_ptr<CubeTexture> texture)
    {
      _luminanceTexture = texture;
      _updateLuminanceTexture();
    });

  positionRotateJoint().addChild(_backgroundRenderer);

  if (_object.visible()) registerCompositeObject(_backgroundRenderer);
}

void BackgroundObserver::updateVisible(bool newVisible) noexcept
{
  Object3DRenderObserver::updateVisible(newVisible);
  _updateBackgroundVisible();
}

void BackgroundObserver::_updateBackgroundVisible() noexcept
{
  try
  {
    if(_object.visible()) registerCompositeObject(_backgroundRenderer);
    else unregisterCompositeObject(_backgroundRenderer);
  }
  catch (std::exception& error)
  {
    Log() << "BackgroundObserver::_updateAreaModificator: unable to update area modificator: " << error.what();
  }
  catch (...)
  {
    Log() << "BackgroundObserver::_updateAreaModificator: unable to update area modificator: unknown error";
  }
}

void BackgroundObserver::_updateLuminanceTexture() noexcept
{
  _backgroundRenderer.setLuminanceTexture(_luminanceTexture);

  if(_light.has_value())
  {
    try
    {
      std::shared_ptr<CubeTexture> diffuseLuminanceMap;
      if(_luminanceTexture != nullptr)
      {
        diffuseLuminanceMap.reset(new CubeTexture(_luminanceTexture->device()));
        uint32_t luminanceMapExtent = std::min( _luminanceTexture->sideExtent(),
                                                uint32_t(16));
        diffuseLuminanceMap->buildDiffuseLuminanceMap(_luminanceTexture,
                                                      luminanceMapExtent);
      }
      _light->setDiffuseLuminanceMap(diffuseLuminanceMap);
      _light->setAmbientMap(_luminanceTexture);
    }
    catch (std::exception& error)
    {
      Log() << "BackgroundObserver::_updateLuminanceTexture: " << error.what();
    }
    catch (...)
    {
      Log() << "BackgroundObserver::_updateLuminanceTexture: unknown error";
    }
  }
}

void BackgroundObserver::_removeLight() noexcept
{
  if (_light.has_value())
  {
    unregisterCompositeObject(_light.value());
    positionRotateJoint().removeChild(_light.value());
    _light.reset();
  }
}

void BackgroundObserver::_updateLight() noexcept
{
  _removeLight();

  if(_object.visible() && _object.lightEnabled())
  {
    try
    {
      _light.emplace( true,
                      true,
                      true,
                      Application::instance().displayDevice());

      positionRotateJoint().addChild(_light.value());
      registerCompositeObject(_light.value());

      _updateLuminance();
      _updateLuminanceTexture();
    }
    catch(std::exception& error)
    {
      _removeLight();
      Log() << "BackgroundObserver::_updateLightDrawable: " << error.what();
    }
    catch(...)
    {
      _removeLight();
      Log() << "BackgroundObserver::_updateLightDrawable: unknown error";
    }
  }
}

void BackgroundObserver::_updateLuminance() noexcept
{
  glm::vec3 newLuminance = _object.color() * _object.luminance();
  _backgroundRenderer.setLuminance(newLuminance);
  if(_light.has_value())
  {
    _light->setIlluminance(glm::pi<float>() * newLuminance);
  }
}

void BackgroundObserver::_updateDissolutionStartDistance() noexcept
{
  _backgroundRenderer.setDissolutionStartDistance(
                                          _object.dissolutionStartDistance());
}

void BackgroundObserver::_updateDissolutionLength() noexcept
{
  _backgroundRenderer.setDissolutionLength(_object.dissolutionLength());
}
