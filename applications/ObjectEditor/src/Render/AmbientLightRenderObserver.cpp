#include <vector>

#include <mtt/dlPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/dlPipeline/constants.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>
#include <mtt/render/Pipeline/Buffer.h>

#include <Objects/AmbientLightObject.h>
#include <Render/AmbientLightRenderObserver.h>
#include <EditorApplication.h>
#include <EditorCommonData.h>

#define ICON_FILE ":/ObjectEditor/ambientLight.png"
#define ICON_SIZE 32

#define CYRCLE_SEGMENTS 32

AmbientLightRenderObserver::AmbientLightRenderObserver(
                                                AmbientLightObject& object,
                                                EditorCommonData& commonData) :
  AbstractLightRenderObserver(object, commonData),
  _lightObject(object),
  _lightDrawable(EditorApplication::instance().displayDevice()),
  _cubemapObserver(object.ambientMap()),
  _sphereMesh(EditorApplication::instance().displayDevice()),
  _iconNode(ICON_FILE, ICON_SIZE)
{
  registerUnculledDrawable(_iconNode);
  positionJoint().addChild(_iconNode);
  _iconNode.addModificator(visibleFilter());
  _iconNode.addModificator(uidSetter());
  _iconNode.addModificator(selectionModificator());

  _sphereMesh.setTechnique(
        mtt::dlPipeline::colorFrameType,
        std::make_unique<mtt::dlPipeline::InstrumentalCompositeTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));
  _sphereMesh.setTechnique( mtt::dlPipeline::uidFrameType,
                            std::make_unique<mtt::UidMeshTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));

  mtt::SurfaceMaterialData materialData;
  materialData.albedo = glm::vec3(.7f, .7f, .4f);
  materialData.roughness = 1;
  materialData.specularStrength = 1;
  _sphereMesh.extraData().setSurfaceMaterialData(materialData);

  _sphereNode.setDrawable(&_sphereMesh, mtt::Sphere());
  registerCulledDrawable(_sphereNode);
  positionRotateJoint().addChild(_sphereNode);
  _sphereNode.addModificator(visibleFilter());
  _sphereNode.addModificator(uidSetter());
  _sphereNode.addModificator(selectionModificator());

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
      _lightDrawable.setDiffuseLuminanceMap(diffuseLuminanceMap);
      _lightDrawable.setAmbientMap(environmentMap);
    });

  setApplicator(_lightDrawable);

  connect(&_lightObject,
          &AmbientLightObject::saturationDistanceChanged,
          this,
          &AmbientLightRenderObserver::_updateSaturationDistance,
          Qt::DirectConnection);
  _updateSaturationDistance();
}

void AmbientLightRenderObserver::updateBounding()
{
  AbstractLightRenderObserver::updateBounding();
  _sphereNode.setLocalBoundSphere(getBoundingSphere());
}

void AmbientLightRenderObserver::updateDistance() noexcept
{
  AbstractLightRenderObserver::updateDistance();
  _updateSphereMesh();
}

void AmbientLightRenderObserver::_updateSaturationDistance() noexcept
{
  _lightDrawable.setSaturationDistance(_lightObject.saturationDistance());
}

void AmbientLightRenderObserver::_updateSphereMesh() noexcept
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

  mtt::LogicalDevice& device = EditorApplication::instance().displayDevice();
  std::shared_ptr<mtt::Buffer> positionsBuffer(
                                  new mtt::Buffer(device,
                                                  mtt::Buffer::VERTEX_BUFFER));
  positionsBuffer->setData( vertices.data(),
                            vertices.size() * sizeof(glm::vec3));
  _sphereMesh.setPositionBuffer(positionsBuffer);
  _sphereMesh.setVerticesNumber(uint32_t(vertices.size()));
}
