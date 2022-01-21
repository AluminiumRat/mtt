#include <mtt/Application/Application.h>
#include <mtt/DLPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/DLPipeline/constants.h>
#include <mtt/Render/Mesh/UidMeshTechnique.h>
#include <mtt/Render/Pipeline/Buffer.h>

#include <Objects/SkeletonObject.h>
#include <Render/SkeletonRenderObserver.h>

SkeletonRenderObserver::SkeletonRenderObserver( SkeletonObject& object,
                                                EditorCommonData& commonData) :
  Object3DRenderObserver(object, commonData),
  _crossMesh(mtt::Application::instance().displayDevice())
{
  mtt::LogicalDevice& device = mtt::Application::instance().displayDevice();

  _crossMesh.setTechnique(
              mtt::DLPipeline::colorFrameType,
              std::make_unique<mtt::DLPipeline::InstrumentalCompositeTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                false,
                                                false));
  _crossMesh.setTechnique(mtt::DLPipeline::uidFrameType,
                          std::make_unique<mtt::UidMeshTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                false,
                                                false));

  glm::vec3 linePositions[] = { {-50.f,   0.f,   0.f},
                                { 50.f,   0.f,   0.f},
                                {  0.f, -50.f,   0.f},
                                {  0.f,  50.f,   0.f},
                                {  0.f,   0.f, -50.f},
                                {  0.0f,  0.f,  50.f}};
  std::shared_ptr<mtt::Buffer> linePositionsBuffer(
                                  new mtt::Buffer(device,
                                                  mtt::Buffer::VERTEX_BUFFER));
  linePositionsBuffer->setData(linePositions, sizeof(linePositions));
  _crossMesh.setPositionBuffer(linePositionsBuffer);
  _crossMesh.setVerticesNumber(6);

  mtt::SurfaceMaterialData materialData;
  materialData.albedo = glm::vec3(.7f, .7f, 0.f);
  materialData.roughness = 1;
  materialData.specularStrength = 1;
  _crossMesh.extraData().setSurfaceMaterialData(materialData);

  _drawableNode.setDrawable(&_crossMesh, mtt::Sphere());
  _drawableNode.registerModificator(_autoscale);

  registerUnculledDrawable(_drawableNode);
  positionRotateJoint().addChild(_drawableNode);
  _drawableNode.registerModificator(visibleFilter());
  _drawableNode.registerModificator(uidSetter());
  _drawableNode.registerModificator(selectionModificator());
}
