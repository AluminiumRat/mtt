#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include <glm/vec2.hpp>

#include <mtt/render/ImageView.h>
#include <mtt/render/Lockable.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class LogicalDevice;
  class AbstractRenderPass;

  class FrameBuffer : public Lockable
  {
  protected:
    /// Only render pass can create frame buffer
    friend class AbstractRenderPass;
    FrameBuffer(AbstractRenderPass& renderPass,
                const std::vector<ImageView*>& attachments,
                const std::vector<ImageView*>& samplerMaps);
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator = (const FrameBuffer&) = delete;
  protected:
    virtual ~FrameBuffer();

  public:
    inline VkFramebuffer handle() const noexcept;
    inline const glm::uvec2& extent() const noexcept;

    inline uint32_t attachmentsNumber() const noexcept;
    inline ImageView& attachment(uint32_t index) noexcept;
    inline const ImageView& attachment(uint32_t index) const noexcept;

    inline size_t samplerMapsNumber() const noexcept;
    inline ImageView& samplerMap(size_t index) noexcept;
    inline const ImageView& samplerMap(size_t index) const noexcept;

    bool isCompatible(const AbstractRenderPass& pass) const noexcept;

  private:
    void _cleanup() noexcept;

  private:
    VkFramebuffer _handle;
    LogicalDevice& _device;
    size_t _compatabilityIndex;
    glm::uvec2 _extent;
    std::vector<Ref<ImageView>> _attachments;
    std::vector<Ref<ImageView>> _samplerMaps;
  };

  inline VkFramebuffer FrameBuffer::handle() const noexcept
  {
    return _handle;
  }

  inline const glm::uvec2& FrameBuffer::extent() const noexcept
  {
    return _extent;
  }

  inline uint32_t FrameBuffer::attachmentsNumber() const noexcept
  {
    return uint32_t(_attachments.size());
  }

  inline ImageView& FrameBuffer::attachment(uint32_t index) noexcept
  {
    return *_attachments[index];
  }

  inline const ImageView& FrameBuffer::attachment(uint32_t index) const noexcept
  {
    return *_attachments[index];
  }

  inline size_t FrameBuffer::samplerMapsNumber() const noexcept
  {
    return _samplerMaps.size();
  }

  inline ImageView& FrameBuffer::samplerMap(size_t index) noexcept
  {
    return *_samplerMaps[index];
  }

  inline const ImageView& FrameBuffer::samplerMap(size_t index) const noexcept
  {
    return *_samplerMaps[index];
  }
}