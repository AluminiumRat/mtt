#include <mtt/clPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/editorLib/Render/IconDrawableNode.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>

using namespace mtt;

IconDrawableNode::IconDrawableNode( const QString& imageFilename,
                                    float iconSize) :
  _iconMesh(EditorApplication::instance().displayDevice())
{
  _iconMesh.setTechnique(
              clPipeline::colorFrameType,
              std::make_unique<clPipeline::InstrumentalCompositeTechnique>(
                                            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                                            true,
                                            false));
  _iconMesh.setTechnique( clPipeline::uidFrameType,
                          std::make_unique<UidMeshTechnique>(
                                            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                                            true,
                                            true));

  EditorApplication& application = EditorApplication::instance();
  LogicalDevice& device = application.displayDevice();

  float halfsize = iconSize / 2;
  glm::vec3 positions[] = { {-halfsize, -halfsize,  0.f},
                            {-halfsize,  halfsize,  0.f},
                            { halfsize,  halfsize,  0.f},
                            {-halfsize, -halfsize,  0.f},
                            { halfsize,  halfsize,  0.f},
                            { halfsize, -halfsize, 0.f}};
  std::shared_ptr<Buffer> positionsBuffer(new Buffer( device,
                                                      Buffer::VERTEX_BUFFER));
  positionsBuffer->setData(positions, sizeof(positions));
  _iconMesh.setPositionBuffer(positionsBuffer);

  glm::vec2 texCoords[] = { { 0.f,  1.f},
                            { 0.f,  0.f},
                            { 1.f,  0.f},
                            { 0.f,  1.f},
                            { 1.f,  0.f},
                            { 1.f,  1.f}};
  std::shared_ptr<Buffer> texCoordBuffer(new Buffer(device,
                                                    Buffer::VERTEX_BUFFER));
  texCoordBuffer->setData(texCoords, sizeof(texCoords));
  _iconMesh.setTexCoordBuffer(texCoordBuffer);

  _iconMesh.setVerticesNumber(6);

  std::shared_ptr<Texture2D> iconTexture =
                                  application.textureLibrary.load(imageFilename,
                                                                  device,
                                                                  true);
  std::unique_ptr<Sampler> colorSampler(
                                new Sampler( PipelineResource::STATIC, device));
  colorSampler->setAttachedTexture(iconTexture);
  _iconMesh.extraData().setAlbedoSampler(std::move(colorSampler));
  _iconMesh.extraData().setAlphaInAlbedoSamplerIsOpacity(true);

  setDrawable(&_iconMesh, Sphere());
  addModificator(_autoscale);
  addModificator(_autorotate);
}
