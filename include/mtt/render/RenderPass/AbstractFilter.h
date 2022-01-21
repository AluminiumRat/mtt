#pragma once

#include <memory>
#include <optional>

#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/RenderPass/AbstractRenderPass.h>

namespace mtt
{
  class AbstractFilter : public AbstractRenderPass
  {
  public:
    AbstractFilter( VkFormat targetFormat,
                    VkImageLayout targetLayout,
                    LogicalDevice& device);
    AbstractFilter(const AbstractFilter&) = delete;
    AbstractFilter& operator = (const AbstractFilter&) = delete;
  protected:
    virtual ~AbstractFilter() noexcept = default;

  public:
    inline VkFormat tagetFormat() const noexcept;
    inline VkImageLayout targetLayout() const noexcept;

    Ref<FrameBuffer> createFrameBuffer(ImageView& target);

    virtual void scheduleDraw(ImageView& srcImage, DrawContext& context);

  protected:
    inline GraphicsPipeline& pipeline() noexcept;
    inline const GraphicsPipeline& pipeline() const noexcept;

  private:
    void _adjustRenderPass();
    void _adjustPipeline();

  private:
    VkFormat _targetFormat;
    VkImageLayout _targetLayout;

    std::optional<GraphicsPipeline> _pipeline;
    Sampler _srcSampler;
    std::shared_ptr<Texture2D> _srcTexture;
  };

  inline VkFormat AbstractFilter::tagetFormat() const noexcept
  {
    return _targetFormat;
  }

  inline VkImageLayout AbstractFilter::targetLayout() const noexcept
  {
    return _targetLayout;
  }

  inline GraphicsPipeline& AbstractFilter::pipeline() noexcept
  {
    return *_pipeline;
  }

  inline const GraphicsPipeline& AbstractFilter::pipeline() const noexcept
  {
    return *_pipeline;
  }
};