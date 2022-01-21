#include <mtt/DLPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/DLPipeline/constants.h>
#include <mtt/Render/Mesh/UidMeshTechnique.h>

#include <Render/IconDrawableNode.h>
#include <EditorApplication.h>

IconDrawableNode::IconDrawableNode( const QString& imageFilename,
                                    float iconSize) noexcept :
  _iconMesh(EditorApplication::instance().displayDevice())
{
  _iconMesh.setTechnique(
              mtt::DLPipeline::colorFrameType,
              std::make_unique<mtt::DLPipeline::InstrumentalCompositeTechnique>(
                                            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                                            true,
                                            false));
  _iconMesh.setTechnique( mtt::DLPipeline::uidFrameType,
                          std::make_unique<mtt::UidMeshTechnique>(
                                            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                                            true,
                                            true));

  EditorApplication& application = EditorApplication::instance();
  mtt::LogicalDevice& device = application.displayDevice();

  float halfsize = iconSize / 2;
  glm::vec3 positions[] = { {-halfsize, -halfsize,  0.f},
                            {-halfsize,  halfsize,  0.f},
                            { halfsize,  halfsize,  0.f},
                            {-halfsize, -halfsize,  0.f},
                            { halfsize,  halfsize,  0.f},
                            { halfsize, -halfsize, 0.f}};
  std::shared_ptr<mtt::Buffer> positionsBuffer(
                                  new mtt::Buffer(device,
                                                  mtt::Buffer::VERTEX_BUFFER));
  positionsBuffer->setData(positions, sizeof(positions));
  _iconMesh.setPositionBuffer(positionsBuffer);

  glm::vec2 texCoords[] = { { 0.f,  1.f},
                            { 0.f,  0.f},
                            { 1.f,  0.f},
                            { 0.f,  1.f},
                            { 1.f,  0.f},
                            { 1.f,  1.f}};
  std::shared_ptr<mtt::Buffer> texCoordBuffer(
                                  new mtt::Buffer(device,
                                                  mtt::Buffer::VERTEX_BUFFER));
  texCoordBuffer->setData(texCoords, sizeof(texCoords));
  _iconMesh.setTexCoordBuffer(texCoordBuffer);

  _iconMesh.setVerticesNumber(6);

  std::shared_ptr<mtt::Texture2D> iconTexture =
                                  application.textureLibrary.load(imageFilename,
                                                                  device,
                                                                  true);
  std::unique_ptr<mtt::Sampler> colorSampler(
                      new mtt::Sampler( mtt::PipelineResource::STATIC, device));
  colorSampler->setAttachedTexture(iconTexture);
  _iconMesh.extraData().setAlbedoSampler(std::move(colorSampler));

  setDrawable(&_iconMesh, mtt::Sphere());
  registerModificator(_autoscale);
  registerModificator(_autorotate);
}
