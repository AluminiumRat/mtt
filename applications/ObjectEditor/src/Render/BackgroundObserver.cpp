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
  _backgroundDrawable(mtt::Application::instance().displayDevice()),
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
  _backgroundDrawableNode.setDrawable(&_backgroundDrawable, mtt::Sphere());
  _backgroundDrawableNode.registerModificator(visibleFilter());
  registerUnculledDrawable(_backgroundDrawableNode);

  positionRotateJoint().addChild(_lightDrawableNode);
  _lightDrawableNode.registerModificator(visibleFilter());
  registerUnculledDrawable(_lightDrawableNode);
}

void BackgroundObserver::_updateLuminanceTexture() noexcept
{
  _backgroundDrawable.setLuminanceTexture(_luminanceTexture);

  if(_lightDrawable.has_value())
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
      _lightDrawable->setDiffuseLuminanceMap(diffuseLuminanceMap);
      _lightDrawable->setAmbientMap(_luminanceTexture);
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
  _lightDrawableNode.setDrawable(nullptr, mtt::Sphere());
  _lightDrawable.reset();

  if(_object.lightEnabled())
  {
    try
    {
      _lightDrawable.emplace(mtt::Application::instance().displayDevice());
      _lightDrawable->setInfinityAreaMode(true);
      _lightDrawableNode.setDrawable(&_lightDrawable.value(), mtt::Sphere());

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
  _backgroundDrawable.setLuminance(newLuminance);
  if(_lightDrawable.has_value())
  {
    _lightDrawable->setIlluminance(glm::pi<float>() * newLuminance);
  }
}

void BackgroundObserver::_updateDissolutionStartDistance() noexcept
{
  _backgroundDrawable.setDissolutionStartDistance(
                                          _object.dissolutionStartDistance());
}

void BackgroundObserver::_updateDissolutionLength() noexcept
{
  _backgroundDrawable.setDissolutionLength(_object.dissolutionLength());
}
