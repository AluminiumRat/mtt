#pragma once

#include <glm/vec2.hpp>

#include <memory>

#include <mtt/render/SceneRenderer/AbstractSceneRenderer.h>
#include <mtt/render/SceneRenderer/OneTargetFrameBuilder.h>

namespace mtt
{
  class CommandQueue;
  class LogicalDevice;

  class SceneToImageRenderer : public AbstractSceneRenderer
  {
  public:
    SceneToImageRenderer( OneTargetFrameBuilder& frameBuilder,
                          CommandQueue& drawQueue);
    SceneToImageRenderer(const SceneToImageRenderer&) = delete;
    SceneToImageRenderer& operator = (const SceneToImageRenderer&) = delete;
    virtual ~SceneToImageRenderer() noexcept = default;

    virtual void render(RenderScene& scene,
                        const CameraNode& camera,
                        ViewInfo* rootViewInfo) override;

    inline const glm::uvec2& frameExtent() const noexcept;
    void setExtent(const glm::uvec2& newExtent) noexcept;

    inline Image* targetImage() noexcept;
    inline const Image* targetImage() const noexcept;

    inline OneTargetFrameBuilder& frameBuilder() const noexcept;

  private:
    CommandQueue& _drawQueue;

    OneTargetFrameBuilder& _frameBuilder;
    glm::uvec2 _frameExtent;
    std::unique_ptr<OneTargetFrameBuilder::Frame> _frame;
  };

  inline const glm::uvec2&
                        SceneToImageRenderer::frameExtent() const noexcept
  {
    return _frameExtent;
  }

  inline Image* SceneToImageRenderer::targetImage() noexcept
  {
    if(_frame == nullptr) return nullptr;
    return &_frame->targetImage();
  }

  inline const Image* SceneToImageRenderer::targetImage() const noexcept
  {
    if (_frame == nullptr) return nullptr;
    return &_frame->targetImage();
  }

  inline OneTargetFrameBuilder&
                            SceneToImageRenderer::frameBuilder() const noexcept
  {
    return _frameBuilder;
  }
}