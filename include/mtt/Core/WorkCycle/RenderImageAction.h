#pragma once

#include <functional>

#include <mtt/Core/WorkCycle/RenderSceneAction.h>
#include <mtt/Render/PlainBuffer.h>
#include <mtt/Render/Ref.h>

namespace mtt
{
  class SceneToImageRenderer;

  class RenderImageAction : public RenderSceneAction
  {
  public:
    using Callback = std::function<void(void* data)>;

  public:
    RenderImageAction(SceneToImageRenderer& renderer,
                      RenderScene& scene,
                      CameraNode& camera,
                      ViewInfo* rootViewInfo,
                      const glm::uvec2& extent,
                      VkImageAspectFlagBits imageAspect,
                      const Callback& callback);
    RenderImageAction(const RenderImageAction&) = delete;
    RenderImageAction& operator = (const RenderImageAction&) = delete;
    virtual ~RenderImageAction() noexcept = default;

    virtual void asycPart() override;
    virtual void postAsycPart() override;

  private:
    SceneToImageRenderer& _renderer;
    glm::uvec2 _extent;
    VkImageAspectFlagBits _imageAspect;
    Callback _callback;
    Ref<PlainBuffer> _downloadBuffer;
  };
}