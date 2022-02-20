#include <mtt/application/Application.h>
#include <mtt/clPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>
#include <mtt/editorLib/Render/SkeletonRenderObserver.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>
#include <mtt/render/Pipeline/Buffer.h>

using namespace mtt;

SkeletonRenderObserver::SkeletonRenderObserver( SkeletonObject& object,
                                                CommonEditData& commonData) :
  Object3DRenderObserver(object, commonData),
  _crossMesh(Application::instance().displayDevice())
{
  LogicalDevice& device = Application::instance().displayDevice();

  _crossMesh.setTechnique(
              clPipeline::colorFrameType,
              std::make_unique<clPipeline::InstrumentalCompositeTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                false,
                                                false));
  _crossMesh.setTechnique(clPipeline::uidFrameType,
                          std::make_unique<UidMeshTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                false,
                                                false));

  glm::vec3 linePositions[] = { {-50.f,   0.f,   0.f},
                                { 50.f,   0.f,   0.f},
                                {  0.f, -50.f,   0.f},
                                {  0.f,  50.f,   0.f},
                                {  0.f,   0.f, -50.f},
                                {  0.0f,  0.f,  50.f}};
  std::shared_ptr<Buffer> linePositionsBuffer(
                                    new Buffer(device, Buffer::VERTEX_BUFFER));
  linePositionsBuffer->setData(linePositions, sizeof(linePositions));
  _crossMesh.setPositionBuffer(linePositionsBuffer);
  _crossMesh.setVerticesNumber(6);

  SurfaceMaterialData materialData;
  materialData.albedo = glm::vec3(.7f, .7f, 0.f);
  materialData.roughness = 1;
  materialData.specularStrength = 1;
  _crossMesh.extraData().setSurfaceMaterialData(materialData);

  _drawableNode.setDrawable(&_crossMesh, Sphere());
  _drawableNode.addModificator(_autoscale);

  registerUnculledDrawable(_drawableNode);
  positionRotateJoint().addChild(_drawableNode);
  _drawableNode.addModificator(visibleFilter());
  _drawableNode.addModificator(uidSetter());
  _drawableNode.addModificator(selectionModificator());
}
