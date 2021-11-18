#pragma once

#include <string>

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#include <mtt/Render/CommandQueue/CommandProducer.h>
#include <mtt/Render/DrawPlan/DrawContext.h>
#include <mtt/Render/DrawPlan/StageIndex.h>
#include <mtt/Render/RenderPass/FrameBuffer.h>
#include <mtt/Utilities/Abort.h>

namespace mtt
{
  class CommandProducer;
  class ImageView;
  class LogicalDevice;

  class AbstractRenderPass : public Lockable
  {
  public:
    static const VkPipelineColorBlendAttachmentState noColorBlendingState;
    static const VkPipelineColorBlendAttachmentState
                                                premultipledColorBlendingState;
    static const VkPipelineColorBlendAttachmentState
                                                  accamulateColorBlendingState;

  public:
    struct SubpassInfo
    {
      uint32_t subpassIndex;
      VkPipelineMultisampleStateCreateInfo multisampling;
      std::vector<VkPipelineColorBlendAttachmentState> blentAttachments;
    };

  public:
    explicit AbstractRenderPass(LogicalDevice& device);
    AbstractRenderPass(const AbstractRenderPass&) = delete;
    AbstractRenderPass& operator = (const AbstractRenderPass&) = delete;
  protected:
    virtual ~AbstractRenderPass() noexcept;

  public:
    inline VkRenderPass handle() const;
    inline LogicalDevice& device() const noexcept;

    inline size_t compatabilityIndex() const noexcept;

    bool stageSupports(StageIndex stage) const noexcept;
    const SubpassInfo& subpassInfo(StageIndex stage) const noexcept;
    inline size_t subpassesNumber() const noexcept;
    inline const SubpassInfo& subpassInfo(size_t subpassIndex) const noexcept;
    inline const std::vector<StageIndex>& subpassStages(
                                          size_t subpassIndex) const noexcept;

  protected:
    struct AttachmentDescription
    {
      VkAttachmentDescription attachmentInfo;
      VkClearValue clearValue;
    };

    struct SubpassDescription
    {
      VkSubpassDescriptionFlags flags = 0;
      VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      std::vector<VkAttachmentReference> inputAttachments;
      std::vector<VkAttachmentReference> colorAttachments;
      std::vector<VkAttachmentReference> resolveAttachments;
      std::optional<VkAttachmentReference> depthStencilAttachment;
      std::vector<uint32_t> preserveAttachments;
      SubpassInfo subpassInfo;
      std::vector<StageIndex> stages;
    };

  protected:
    virtual void setup( const std::vector<VkSubpassDependency>& dependencies,
                        const std::vector<AttachmentDescription>& attachments,
                        size_t samplerMapsNumber,
                        const std::vector<SubpassDescription>& subpasses);

    void fillMultisampling(SubpassInfo& info) const noexcept;

    Ref<FrameBuffer> createFrameBuffer(
                                    const std::vector<ImageView*>& attachments,
                                    const std::vector<ImageView*>& samplerMaps);

    /// DrawDelegate is functional object with signature
    /// void(DrawContext& context, size_t subpassIndex)
    template <typename DrawDelegate>
    inline void scheduleDrawT(DrawContext& context, DrawDelegate& drawDelegate);

  private:
    void _updateCompatibility();
    void _createHandle() const;
    void _cleanup() noexcept;

  private:
    mutable VkRenderPass _handle;
    LogicalDevice& _device;

    std::vector<VkSubpassDependency> _dependencies;
    std::vector<VkAttachmentDescription> _attachmentsInfo;
    size_t _samplerMapsNumber;
    std::vector<VkClearValue> _clearValues;
    std::vector<SubpassDescription> _subpasses;

    size_t _compatibilityIndex;
  };

  inline VkRenderPass AbstractRenderPass::handle() const
  {
    if(_handle == VK_NULL_HANDLE) _createHandle();
    if (_handle == VK_NULL_HANDLE) Abort("AbstractRenderPass::handle: createHandle returned null.");
    return _handle;
  }

  inline LogicalDevice& AbstractRenderPass::device() const noexcept
  {
    return _device;
  }

  inline size_t AbstractRenderPass::compatabilityIndex() const noexcept
  {
    return _compatibilityIndex;
  }

  inline size_t AbstractRenderPass::subpassesNumber() const noexcept
  {
    return _subpasses.size();
  }

  inline const AbstractRenderPass::SubpassInfo&
            AbstractRenderPass::subpassInfo(size_t subpassIndex) const noexcept
  {
    return _subpasses[subpassIndex].subpassInfo;
  }

  inline const std::vector<StageIndex>&
                          AbstractRenderPass::subpassStages(
                                            size_t subpassIndex) const noexcept
  {
    return _subpasses[subpassIndex].stages;
  }

  template <typename DrawDelegate>
  inline void AbstractRenderPass::scheduleDrawT(DrawContext& context,
                                                DrawDelegate& drawDelegate)
  {
    FrameBuffer* frameBuffer = context.frameBuffer;
    if (frameBuffer == nullptr || !frameBuffer->isCompatible(*this)) Abort("AbstractRenderPass::scheduleRender: the target frame is incompatible with this render pass.");

    std::vector<VkImageLayout> samplerMapLayouts;
    samplerMapLayouts.reserve(frameBuffer->samplerMapsNumber());
    for(size_t samplerMapIndex = 0;
        samplerMapIndex < frameBuffer->samplerMapsNumber();
        samplerMapIndex++)
    {
      samplerMapLayouts.push_back(
                          frameBuffer->samplerMap(samplerMapIndex).image().primaryLayout());
      frameBuffer->samplerMap(samplerMapIndex).image().scheduleChangePrimaryLayout(
                    frameBuffer->samplerMap(samplerMapIndex).subresourceRange().aspectMask,
                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                    context.drawProducer);
    }

    VkCommandBuffer commandBuffer = context.drawProducer.bufferHandle();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = handle();
    renderPassInfo.framebuffer = frameBuffer->handle();
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent.width = frameBuffer->extent().x;
    renderPassInfo.renderArea.extent.height = frameBuffer->extent().y;

    renderPassInfo.clearValueCount = uint32_t(_clearValues.size());
    renderPassInfo.pClearValues = _clearValues.data();

    vkCmdBeginRenderPass( commandBuffer,
                          &renderPassInfo,
                          VK_SUBPASS_CONTENTS_INLINE);

    for(size_t subpassIndex = 0;
        subpassIndex < _subpasses.size();
        subpassIndex++)
    {
      drawDelegate(context, subpassIndex);
      if(subpassIndex != _subpasses.size() - 1)
      {
        vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
      }
    }

    vkCmdEndRenderPass(commandBuffer);

    for(size_t samplerMapIndex = 0;
        samplerMapIndex < frameBuffer->samplerMapsNumber();
        samplerMapIndex++)
    {
      frameBuffer->samplerMap(samplerMapIndex).image().scheduleChangePrimaryLayout(
                    frameBuffer->samplerMap(samplerMapIndex).subresourceRange().aspectMask,
                    samplerMapLayouts[samplerMapIndex],
                    context.drawProducer);
    }

    context.drawProducer.lockResource(*frameBuffer);
    context.drawProducer.lockResource(*this);
  }
}