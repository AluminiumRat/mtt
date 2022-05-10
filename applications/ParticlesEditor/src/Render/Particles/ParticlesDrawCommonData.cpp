#include <memory>

#include <mtt/Application/Application.h>

#include <Render/Particles/ParticlesDrawCommonData.h>

ParticlesDrawCommonData::ParticlesDrawCommonData() :
  positionBuffer( mtt::Application::instance().displayDevice(),
                  mtt::Buffer::VERTEX_BUFFER),
  sizeRotationBuffer( mtt::Application::instance().displayDevice(),
                      mtt::Buffer::VERTEX_BUFFER),
  albedoBuffer( mtt::Application::instance().displayDevice(),
                mtt::Buffer::VERTEX_BUFFER),
  emissionBuffer( mtt::Application::instance().displayDevice(),
                  mtt::Buffer::VERTEX_BUFFER),
  textureIndexBuffer( mtt::Application::instance().displayDevice(),
                      mtt::Buffer::VERTEX_BUFFER),
  tileIndexBuffer(mtt::Application::instance().displayDevice(),
                  mtt::Buffer::VERTEX_BUFFER),
  falloffFactorBuffer(mtt::Application::instance().displayDevice(),
                      mtt::Buffer::VERTEX_BUFFER),
  particlesNumber(0),
  depthSampler( mtt::PipelineResource::VOLATILE,
                mtt::Application::instance().displayDevice()),
  depthSamplerTexture(nullptr),
  falloffBaseMppx(.005f),
  falloffSmoothing(1.f)
{
  std::shared_ptr<mtt::Texture2D> depthTexture =
                        std::make_shared<mtt::Texture2D>(
                                  mtt::Application::instance().displayDevice());
  depthSamplerTexture = depthTexture.get();
  depthSampler.setAttachedTexture(std::move(depthTexture));
}
