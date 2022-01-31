#include <vector>

#include <mtt/clPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>
#include <mtt/render/Pipeline/Buffer.h>

#include <Objects/DirectLightObject.h>
#include <Render/DirectLightRenderObserver.h>
#include <EditorApplication.h>
#include <EditorCommonData.h>

#define ICON_FILE ":/ObjectEditor/directLight.png"
#define ICON_SIZE 32

#define CAP_SEGMENTS 32
#define BODY_SEGMENTS 6

DirectLightRenderObserver::DirectLightRenderObserver(
                                                DirectLightObject& object,
                                                EditorCommonData& commonData) :
  AbstractLightRenderObserver(object, commonData),
  _lightObject(object),
  _lightDrawable(EditorApplication::instance().displayDevice()),
  _cylinderMesh(EditorApplication::instance().displayDevice()),
  _iconNode(ICON_FILE, ICON_SIZE)
{
  registerUnculledDrawable(_iconNode);
  positionJoint().addChild(_iconNode);
  _iconNode.addModificator(visibleFilter());
  _iconNode.addModificator(uidSetter());
  _iconNode.addModificator(selectionModificator());

  _cylinderMesh.setTechnique(
      mtt::clPipeline::colorFrameType,
      std::make_unique<mtt::clPipeline::InstrumentalCompositeTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));
  _cylinderMesh.setTechnique( mtt::clPipeline::uidFrameType,
                              std::make_unique<mtt::UidMeshTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));

  mtt::SurfaceMaterialData materialData;
  materialData.albedo = glm::vec3(.7f, .7f, .4f);
  materialData.roughness = 1;
  materialData.specularStrength = 1;
  _cylinderMesh.extraData().setSurfaceMaterialData(materialData);

  _cylinderNode.setDrawable(&_cylinderMesh, mtt::Sphere());
  registerCulledDrawable(_cylinderNode);
  positionRotateJoint().addChild(_cylinderNode);
  _cylinderNode.addModificator(visibleFilter());
  _cylinderNode.addModificator(uidSetter());
  _cylinderNode.addModificator(selectionModificator());

  setApplicator(_lightDrawable);

  connect(&_lightObject,
          &DirectLightObject::radiusChanged,
          this,
          &DirectLightRenderObserver::_updateRadius,
          Qt::DirectConnection);

  _updateRadius();

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
          &DirectLightObject::blurSizeChanged,
          this,
          &DirectLightRenderObserver::_updateBlur,
          Qt::DirectConnection);
  _updateBlur();
}

void DirectLightRenderObserver::updateBounding()
{
  AbstractLightRenderObserver::updateBounding();
  _cylinderNode.setLocalBoundSphere(getBoundingSphere());
}

mtt::Sphere DirectLightRenderObserver::getBoundingSphere() const noexcept
{
  float halfDistance = _lightObject.distance() / 2;
  float sphereRadius = sqrt(halfDistance * halfDistance +
                                _lightObject.radius() * _lightObject.radius());
  return mtt::Sphere(glm::vec3(0, 0, -halfDistance), sphereRadius);
}

void DirectLightRenderObserver::updateDistance() noexcept
{
  AbstractLightRenderObserver::updateDistance();
  _updateDepthCamera();
  _updateCylinderMesh();
}

void DirectLightRenderObserver::_updateRadius() noexcept
{
  try
  {
    _lightDrawable.setRadius(_lightObject.radius());
    updateBounding();
  }
  catch(const std::exception& error)
  {
    mtt::Log() << "AbstractLightRenderObserver::_updateRadius: unable to update radius: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "AbstractLightRenderObserver::_updateRadius: unable to update radius: unknown error";
  }

  _updateDepthCamera();
  _updateCylinderMesh();
}

void DirectLightRenderObserver::_updateCylinderMesh() noexcept
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

  mtt::LogicalDevice& device = EditorApplication::instance().displayDevice();
  std::shared_ptr<mtt::Buffer> positionsBuffer(
                                  new mtt::Buffer(device,
                                                  mtt::Buffer::VERTEX_BUFFER));
  positionsBuffer->setData( vertices.data(),
                            vertices.size() * sizeof(glm::vec3));
  _cylinderMesh.setPositionBuffer(positionsBuffer);
  _cylinderMesh.setVerticesNumber(uint32_t(vertices.size()));
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
    mtt::LogicalDevice& device = EditorApplication::instance().displayDevice();
    _shadowMapProvider.reset(new mtt::clPipeline::ShadowMapProvider(
                                                          2,
                                                          glm::uvec2(256, 256),
                                                          device));

    _lightDrawable.setShadowMapProvider(_shadowMapProvider.get());
    positionRotateJoint().addChild(_shadowMapProvider->camera());
    _shadowMapProvider->setTargetField(
                                      &commonData().renderScene().culledData());
    _updateDepthCamera();
    _updateShadowMapSize();
  }

  if(!_lightObject.shadowsEnabled() && _shadowMapProvider != nullptr)
  {
    _lightDrawable.setShadowMapProvider(nullptr);
    positionRotateJoint().removeChild(_shadowMapProvider->camera());
    _shadowMapProvider.reset();
  }
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
  _lightDrawable.setCascadeSize(_lightObject.cascadeSize());
}

void DirectLightRenderObserver::_updateBlur() noexcept
{
  _lightDrawable.setBlurSize(_lightObject.blurSize());
}
