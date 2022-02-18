#include <mtt/editorLib/Objects/DirectLightObject.h>
#include <mtt/editorLib/EditorApplication.h>

#include <Render/DirectLightRenderObserver.h>
#include <EditorCommonData.h>

#define ICON_FILE ":/ObjectEditor/directLight.png"
#define ICON_SIZE 32

#define CAP_SEGMENTS 32
#define BODY_SEGMENTS 6

DirectLightRenderObserver::DirectLightRenderObserver(
                                                mtt::DirectLightObject& object,
                                                EditorCommonData& commonData) :
  AbstractLightRenderObserver(object, commonData, ICON_FILE, ICON_SIZE),
  _lightObject(object),
  _light(true, true, mtt::EditorApplication::instance().displayDevice())
{
  setLightObject(_light);

  connect(&_lightObject,
          &mtt::DirectLightObject::baseIlluminanceChanged,
          this,
          &DirectLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);
  connect(&_lightObject,
          &mtt::DirectLightObject::colorChanged,
          this,
          &DirectLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);
  _updateIlluminance();

  connect(&_lightObject,
          &mtt::DirectLightObject::distanceChanged,
          this,
          &DirectLightRenderObserver::_updateDistance,
          Qt::DirectConnection);
  _updateDistance();

  connect(&_lightObject,
          &mtt::DirectLightObject::radiusChanged,
          this,
          &DirectLightRenderObserver::_updateRadius,
          Qt::DirectConnection);
  _updateRadius();

  connect(&_lightObject,
          &mtt::DirectLightObject::shadowsEnabledChanged,
          this,
          &DirectLightRenderObserver::_updateShadowsEnabled,
          Qt::DirectConnection);
  _updateShadowsEnabled();

  connect(&_lightObject,
          &mtt::DirectLightObject::shadowmapSizeChanged,
          this,
          &DirectLightRenderObserver::_updateShadowMapSize,
          Qt::DirectConnection);
  _updateShadowMapSize();

  connect(&_lightObject,
          &mtt::DirectLightObject::cascadeSizeChanged,
          this,
          &DirectLightRenderObserver::_updateCascadeSize,
          Qt::DirectConnection);
  _updateCascadeSize();

  connect(&_lightObject,
          &mtt::DirectLightObject::blurSizeChanged,
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
  try
  {
    std::vector<glm::vec3> vertices;
    vertices.reserve(BODY_SEGMENTS * 2 + CAP_SEGMENTS * 2 * 2);

    float radius = _lightObject.radius();
    float length = _lightObject.distance();

    float bodyStep = 2.f * glm::pi<float>() / BODY_SEGMENTS;
    float bodyAngle = 0;
    for(size_t bodySegment = 0; bodySegment < BODY_SEGMENTS; bodySegment++)
    {
      vertices.emplace_back(cos(bodyAngle) * radius,
                            sin(bodyAngle) * radius,
                            0.f);
      vertices.emplace_back(cos(bodyAngle) * radius,
                            sin(bodyAngle) * radius,
                            -length);
      bodyAngle += bodyStep;
    }

    float capStep = 2.f * glm::pi<float>() / CAP_SEGMENTS;
    float capAngle = 0;
    for(size_t capSegment = 0; capSegment < CAP_SEGMENTS - 1; capSegment++)
    {
      float nextAngle = capAngle + capStep;

      vertices.emplace_back(cos(capAngle) * radius,
                            sin(capAngle) * radius,
                            0.f);
      vertices.emplace_back(cos(nextAngle) * radius,
                            sin(nextAngle) * radius,
                            0.f);

      vertices.emplace_back(cos(capAngle) * radius,
                            sin(capAngle) * radius,
                            -length);
      vertices.emplace_back(cos(nextAngle) * radius,
                            sin(nextAngle) * radius,
                            -length);

      capAngle = nextAngle;
    }

    vertices.emplace_back(cos(capAngle) * radius,
                          sin(capAngle) * radius,
                          0.f);
    vertices.emplace_back(radius, 0.f, 0.f);

    vertices.emplace_back(cos(capAngle) * radius,
                          sin(capAngle) * radius,
                          -length);
    vertices.emplace_back(radius, 0.f, -length);

    setHullGeometry(vertices);
  }
  catch (std::exception& error)
  {
    mtt::Log() << "DirectLightRenderObserver::_updateCylinderMesh: unable to update cylinder mesh: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "DirectLightRenderObserver::_updateCylinderMesh: unable to update cylinder mesh: unknown error.";
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
      mtt::LogicalDevice& device =
                            mtt::EditorApplication::instance().displayDevice();
      _shadowMapProvider.reset(new mtt::clPipeline::ShadowMapProvider(
                                                          2,
                                                          glm::uvec2(256, 256),
                                                          device));

      _light.setShadowMapProvider(_shadowMapProvider.get());
      positionRotateJoint().addChild(_shadowMapProvider->camera());
      _shadowMapProvider->setTargetField(
                                      &commonData().renderScene().culledData());
      _updateDepthCamera();
      _updateShadowMapSize();
    }
    catch (std::exception& error)
    {
      _removeShadowmapProvider();
      mtt::Log() << "DirectLightRenderObserver::_updateShadowsEnabled: unable to update shadow map provider: " << error.what();
    }
    catch (...)
    {
      _removeShadowmapProvider();
      mtt::Log() << "DirectLightRenderObserver::_updateShadowsEnabled: unable to update shadow map provider: unknown error.";
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
