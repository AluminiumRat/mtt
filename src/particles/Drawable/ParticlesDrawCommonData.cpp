#include <memory>

#include <mtt/particles/Drawable/ParticlesDrawCommonData.h>

using namespace mtt;

ParticlesDrawCommonData::ParticlesDrawCommonData(LogicalDevice& theDevice) :
  device(theDevice),
  positionBuffer(device, Buffer::VERTEX_BUFFER),
  sizeRotationBuffer(device, Buffer::VERTEX_BUFFER),
  albedoBuffer(device, Buffer::VERTEX_BUFFER),
  emissionBuffer(device, Buffer::VERTEX_BUFFER),
  textureIndexBuffer(device, Buffer::VERTEX_BUFFER),
  tileIndexBuffer(device, Buffer::VERTEX_BUFFER),
  falloffFactorBuffer(device, Buffer::VERTEX_BUFFER),
  particlesNumber(0),
  depthSampler(PipelineResource::VOLATILE, device),
  depthSamplerTexture(nullptr),
  falloffBaseMppx(.005f),
  falloffSmoothing(1.f),
  lightingType(PER_PARTICLE_LIGHTING)
{
  std::shared_ptr<Texture2D> depthTexture = std::make_shared<Texture2D>(device);
  depthSamplerTexture = depthTexture.get();
  depthSampler.setAttachedTexture(std::move(depthTexture));
}
