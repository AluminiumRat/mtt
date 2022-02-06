#include <stdexcept>

#include <mtt/application/Application.h>
#include <mtt/utilities/Log.h>

#include <AsyncTasks/UploadCubetextureTask.h>
#include <Objects/BackgroundObject.h>
#include <Render/BackgroundObserver.h>
#include <EditorApplication.h>

BackgroundObserver::BackgroundObserver( BackgroundObject& object,
                                        EditorCommonData& commonData) :
  Object3DRenderObserver(object, commonData),
  _object(object),
  _backgroundRenderer(mtt::Application::instance().displayDevice()),
  _lightDrawable(nullptr),
  _cubemapObserver(object.cubemap())
{
  connect(&_object,
          &BackgroundObject::lightEnabledChanged,
          this,
          &BackgroundObserver::_updateLightDrawable,
          Qt::DirectConnection);
  _updateLightDrawable();

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
    [&](std::shared_ptr<mtt::CubeTexture> texture)
    {
      _luminanceTexture = texture;
      _updateLuminanceTexture();
    });

  positionRotateJoint().addChild(_backgroundDrawableNode);
  _backgroundDrawableNode.setDrawable(&_backgroundRenderer.drawable(),
                                      mtt::Sphere());
  _backgroundDrawableNode.addModificator(visibleFilter());
  registerUnculledDrawable(_backgroundDrawableNode);

  positionRotateJoint().addChild(_backgroundRenderer.areaModificator());
  _updateAreaModificator();
}

void BackgroundObserver::updateVisible(bool newVisible) noexcept
{
  Object3DRenderObserver::updateVisible(newVisible);
  _updateAreaModificator();
}

void BackgroundObserver::_updateAreaModificator() noexcept
{
  try
  {
    if(_object.visible())
    {
      registerAreaModificator(_backgroundRenderer.areaModificator());
    }
    else
    {
      unregisterAreaModificator(_backgroundRenderer.areaModificator());
    }
  }
  catch (std::exception& error)
  {
    mtt::Log() << "BackgroundObserver::_updateAreaModificator: unable to update area modificator: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "BackgroundObserver::_updateAreaModificator: unable to update area modificator: unknown error";
  }
}

void BackgroundObserver::_updateLuminanceTexture() noexcept
{
  _backgroundRenderer.setLuminanceTexture(_luminanceTexture);

  if(_light.has_value())
  {
    try
    {
      std::shared_ptr<mtt::CubeTexture> diffuseLuminanceMap;
      if(_luminanceTexture != nullptr)
      {
        diffuseLuminanceMap.reset(
                            new mtt::CubeTexture(_luminanceTexture->device()));
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
      mtt::Log() << "BackgroundObserver::_updateLuminanceTexture: " << error.what();
    }
    catch (...)
    {
      mtt::Log() << "BackgroundObserver::_updateLuminanceTexture: unknown error";
    }
  }
}

void BackgroundObserver::_updateLightDrawable() noexcept
{
  unregisterUnculledDrawable(*_lightDrawable);
  _lightDrawable = nullptr;
  _light.reset();

  if(_object.lightEnabled())
  {
    try
    {
      _light.emplace(mtt::Application::instance().displayDevice());
      _light->setInfinityAreaMode(true);

      _lightDrawable = _light->defferedLightApplicator();
      if(_lightDrawable != nullptr)
      {
        positionRotateJoint().addChild(*_lightDrawable);
        _lightDrawable->addModificator(visibleFilter());
        registerUnculledDrawable(*_lightDrawable);
      }

      _updateLuminance();
      _updateLuminanceTexture();
    }
    catch(std::exception& error)
    {
      mtt::Log() << "BackgroundObserver::_updateLightDrawable: " << error.what();
    }
    catch(...)
    {
      mtt::Log() << "BackgroundObserver::_updateLightDrawable: unknown error";
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
