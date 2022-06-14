#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/AsyncTasks/UploadTextureTask.h>
#include <mtt/editorLib/Objects/SpotLightObject.h>
#include <mtt/editorLib/Render/SpotLightRenderObserver.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/RenderScene.h>

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
          &SpotLightObject::blurAngleChanged,
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
  _light.setAngle(_lightObject.angle());
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
      _shadowMapProvider->setTargetField(&_renderScene.culledData());
      _updateShadowMapSize();
    }
    catch (std::exception& error)
    {
      _removeShadowmapProvider();
      Log() << "SpotLightRenderObserver::_updateShadowsEnabled: unable to update shadow map provider: " << error.what();
    }
    catch (...)
    {
      _removeShadowmapProvider();
      Log() << "SpotLightRenderObserver::_updateShadowsEnabled: unable to update shadow map provider: unknown error.";
    }
  }

  if(!_lightObject.shadowsEnabled() && _shadowMapProvider != nullptr)
  {
    _removeShadowmapProvider();
  }
}

void SpotLightRenderObserver::_removeShadowmapProvider() noexcept
{
  _light.setShadowMapProvider(nullptr);
  _shadowMapProvider.reset();
}

void SpotLightRenderObserver::_updateShadowMapSize() noexcept
{
  if (_shadowMapProvider == nullptr) return;
  uint frameSize = std::max(1u, uint(_lightObject.shadowmapSize()));
  glm::uvec2 frameExtent(frameSize, frameSize);
  _shadowMapProvider->setFrameExtent(frameExtent);
}

void SpotLightRenderObserver::_updateBlur() noexcept
{
  _light.setBlurAngle(_lightObject.blurAngle());
}
