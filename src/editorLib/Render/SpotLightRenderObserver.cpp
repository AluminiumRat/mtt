#include <stdexcept>

#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/AsyncTasks/UploadTextureTask.h>
#include <mtt/editorLib/Objects/SpotLightObject.h>
#include <mtt/editorLib/Render/SpotLightRenderObserver.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/RenderScene.h>
#include <mtt/utilities/Log.h>

#define ICON_FILE ":/editorLib/spotLight.png"
#define ICON_SIZE 32

#define CAP_SEGMENTS 32
#define BODY_SEGMENTS 6

using namespace mtt;

SpotLightRenderObserver::SpotLightRenderObserver( SpotLightObject& object,
                                                  CommonEditData& commonData,
                                                  RenderScene& renderScene) :
  AbstractLightRenderObserver(object, commonData, ICON_FILE, ICON_SIZE),
  _lightObject(object),
  _light(true, true, EditorApplication::instance().displayDevice()),
  _renderScene(renderScene)
{
  setLightObject(_light);

  connect(&_lightObject,
          &SpotLightObject::baseIlluminanceChanged,
          this,
          &SpotLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);
  connect(&_lightObject,
          &SpotLightObject::colorChanged,
          this,
          &SpotLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);
  _updateIlluminance();

  connect(&_lightObject,
          &SpotLightObject::distanceChanged,
          this,
          &SpotLightRenderObserver::_updateDistance,
          Qt::DirectConnection);
  _updateDistance();

  connect(&_lightObject,
          &SpotLightObject::angleChanged,
          this,
          &SpotLightRenderObserver::_updateAngle,
          Qt::DirectConnection);
  _updateAngle();

  connect(&_lightObject,
          &SpotLightObject::filterImageChanged,
          this,
          &SpotLightRenderObserver::_updateFilterImage,
          Qt::DirectConnection);
  _updateFilterImage();

  connect(&_lightObject,
          &SpotLightObject::shadowsEnabledChanged,
          this,
          &SpotLightRenderObserver::_updateShadowsEnabled,
          Qt::DirectConnection);
  _updateShadowsEnabled();

  connect(&_lightObject,
          &SpotLightObject::shadowmapSizeChanged,
          this,
          &SpotLightRenderObserver::_updateShadowMapSize,
          Qt::DirectConnection);
  _updateShadowMapSize();

  connect(&_lightObject,
          &SpotLightObject::shadowBlurChanged,
          this,
          &SpotLightRenderObserver::_updateBlur,
          Qt::DirectConnection);
  _updateBlur();
}

void SpotLightRenderObserver::_updateIlluminance() noexcept
{
  _light.setIlluminance(_lightObject.baseIlluminance() * _lightObject.color());
}

void SpotLightRenderObserver::_updateDistance() noexcept
{
  _light.setDistance(_lightObject.distance());
  _updateConeMesh();
}

void SpotLightRenderObserver::_updateAngle() noexcept
{
  try
  {
    _light.setAngle(_lightObject.angle());
  }
  catch (std::exception& error)
  {
    Log() << "SpotLightRenderObserver::_updateAngle: " << error.what();
  }
  catch (...)
  {
    Log() << "SpotLightRenderObserver::_updateAngle: unknown error";
  }
  _updateConeMesh();
}

void SpotLightRenderObserver::_updateConeMesh() noexcept
{
  if (_lightObject.angle() <= 0.f || _lightObject.distance() <= 0.f)
  {
    hullNode().resetGeometry();
    return;
  }

  try
  {
    float height = _lightObject.distance() * cos(_lightObject.angle() / 2.f);
    float radius = height * tan(_lightObject.angle() / 2.f);
    hullNode().setConeGeometry( radius,
                                height,
                                0.f,
                                BODY_SEGMENTS,
                                CAP_SEGMENTS);
  }
  catch (std::exception& error)
  {
    Log() << "SpotLightRenderObserver::_updateConeMesh: unable to update mesh: " << error.what();
  }
  catch (...)
  {
    Log() << "SpotLightRenderObserver::_updateConeMesh: unable to update mesh: unknown error.";
  }
}

void SpotLightRenderObserver::_updateFilterImage() noexcept
{
  try
  {
    QString filename = _lightObject.filterImage();
    if (filename.isEmpty())
    {
      _light.setFilterTexture(nullptr);
      return;
    }

    auto setTextureFunction =
      [&](std::shared_ptr<mtt::Texture2D> texture)
      {
        _light.setFilterTexture(texture);
      };

    std::unique_ptr<mtt::UploadTextureTask> task;
    task.reset(new mtt::UploadTextureTask(filename, setTextureFunction));
    mtt::AsyncTaskQueue& queue =
                              mtt::EditorApplication::instance().asyncTaskQueue;
    _filterLoadStopper = queue.addTaskWithStopper(std::move(task));
  }
  catch (std::exception& error)
  {
    mtt::Log() << "SpotLightRenderObserver::_updateFilterImage: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "SpotLightRenderObserver::_updateFilterImage: unknown error.";
  }
}

void SpotLightRenderObserver::_updateShadowsEnabled() noexcept
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
    Log() << "SpotLightRenderObserver::_updateShadowsEnabled: " << error.what();
  }
  catch (...)
  {
    Log() << "SpotLightRenderObserver::_updateShadowsEnabled: unknown error";
  }
}

void SpotLightRenderObserver::_updateShadowMapSize() noexcept
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
    Log() << "SpotLightRenderObserver::_updateShadowMapSize: " << error.what();
  }
  catch (...)
  {
    Log() << "SpotLightRenderObserver::_updateShadowMapSize: unknown error";
  }
}

void SpotLightRenderObserver::_updateBlur() noexcept
{
  try
  {
    _light.setBlurAngle(_lightObject.shadowBlur());
  }
  catch (std::exception& error)
  {
    Log() << "SpotLightRenderObserver::_updateBlur: " << error.what();
  }
  catch (...)
  {
    Log() << "SpotLightRenderObserver::_updateBlur: unknown error";
  }
}
