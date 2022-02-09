#include <Objects/AmbientLightObject.h>
#include <Render/AmbientLightRenderObserver.h>
#include <EditorApplication.h>

#define ICON_FILE ":/ObjectEditor/ambientLight.png"
#define ICON_SIZE 32

#define CYRCLE_SEGMENTS 32

AmbientLightRenderObserver::AmbientLightRenderObserver(
                                                AmbientLightObject& object,
                                                EditorCommonData& commonData) :
  AbstractLightRenderObserver(object, commonData, ICON_FILE, ICON_SIZE),
  _lightObject(object),
  _light(true, true, EditorApplication::instance().displayDevice()),
  _cubemapObserver(_lightObject.ambientMap())
{
  mtt::SurfaceMaterialData materialData;
  materialData.albedo = glm::vec3(.7f, .7f, .4f);
  materialData.roughness = 1;
  materialData.specularStrength = 1;

  setLightObject(_light);

  _cubemapObserver.setCallback(
    [&](std::shared_ptr<mtt::CubeTexture> environmentMap)
    {
      std::shared_ptr<mtt::CubeTexture> diffuseLuminanceMap;
      if(environmentMap != nullptr)
      {
        diffuseLuminanceMap.reset(
                                new mtt::CubeTexture(environmentMap->device()));
        uint32_t luminanceMapExtent = std::min( environmentMap->sideExtent(),
                                                uint32_t(16));
        diffuseLuminanceMap->buildDiffuseLuminanceMap(environmentMap,
                                                      luminanceMapExtent);
      }
      _light.setDiffuseLuminanceMap(diffuseLuminanceMap);
      _light.setAmbientMap(environmentMap);
    });

  connect(&_lightObject,
          &AmbientLightObject::baseIlluminanceChanged,
          this,
          &AmbientLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);
  connect(&_lightObject,
          &AmbientLightObject::colorChanged,
          this,
          &AmbientLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);
  _updateIlluminance();

  connect(&_lightObject,
          &AmbientLightObject::distanceChanged,
          this,
          &AmbientLightRenderObserver::_updateDistance,
          Qt::DirectConnection);
  _updateDistance();

  connect(&_lightObject,
          &AmbientLightObject::saturationDistanceChanged,
          this,
          &AmbientLightRenderObserver::_updateSaturationDistance,
          Qt::DirectConnection);
  _updateSaturationDistance();
}

void AmbientLightRenderObserver::_updateIlluminance() noexcept
{
  _light.setIlluminance(_lightObject.color() * _lightObject.baseIlluminance());
}

void AmbientLightRenderObserver::_updateDistance() noexcept
{
  _light.setDistance(_lightObject.distance());
  _updateSphereMesh();
}

void AmbientLightRenderObserver::_updateSphereMesh() noexcept
{
  try
  {
    std::vector<glm::vec3> vertices;
    vertices.reserve(CYRCLE_SEGMENTS * 2 * 3);

    float radius = _lightObject.distance();

    float angleStep = 2.f * glm::pi<float>() / CYRCLE_SEGMENTS;
    float currentAngle = 0;
    for(size_t segment = 0; segment < CYRCLE_SEGMENTS - 1; segment++)
    {
      float nextAngle = currentAngle + angleStep;

      float sinCurrent = sin(currentAngle) * radius;
      float cosCurrent = cos(currentAngle) * radius;
      float sinNext = sin(nextAngle) * radius;
      float cosNext = cos(nextAngle) * radius;

      vertices.emplace_back(cosCurrent, sinCurrent, 0.f);
      vertices.emplace_back(cosNext, sinNext, 0.f);
      vertices.emplace_back(cosCurrent, 0.f, sinCurrent);
      vertices.emplace_back(cosNext, 0.f, sinNext);
      vertices.emplace_back(0.f, cosCurrent, sinCurrent);
      vertices.emplace_back(0.f, cosNext, sinNext);

      currentAngle = nextAngle;
    }

    float sinCurrent = sin(currentAngle) * radius;
    float cosCurrent = cos(currentAngle) * radius;
    vertices.emplace_back(cosCurrent, sinCurrent, 0.f);
    vertices.emplace_back(radius, 0.f, 0.f);
    vertices.emplace_back(cosCurrent, 0.f, sinCurrent);
    vertices.emplace_back(radius, 0.f, 0.f);
    vertices.emplace_back(0.f, cosCurrent, sinCurrent);
    vertices.emplace_back(0.f, radius, 0.f);

    setHullGeometry(vertices);
  }
  catch (std::exception& error)
  {
    mtt::Log() << "AmbientLightRenderObserver::_updateSphereMesh: unable to update sphere mesh: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "AmbientLightRenderObserver::_updateSphereMesh: unable to update sphere mesh: unknown error.";
  }
}

void AmbientLightRenderObserver::_updateSaturationDistance() noexcept
{
  _light.setSaturationDistance(_lightObject.saturationDistance());
}
