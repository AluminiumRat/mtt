#pragma once

#include <mtt/Render/RenderPass/GeneralRenderPass.h>

namespace mtt
{
  class ImageView;

  class PeelingGPass : public GeneralRenderPass
  {
  public:
    static constexpr uint32_t depthAttachmentIndex = 0;
    static constexpr uint32_t normalAttachmentIndex = 1;
    static constexpr uint32_t albedoAttachmentIndex = 2;
    static constexpr uint32_t specularAttachmentIndex = 3;
    static constexpr uint32_t frontAttachmentIndex = 4;
    static constexpr uint32_t backAttachmentIndex = 5;
    static constexpr size_t attachmentNumber = 6;
  
  public:
    PeelingGPass( VkFormat depthMapFormat,
                  VkImageLayout depthMapLayout,
                  VkFormat normalMapFormat,
                  VkImageLayout normalMapLayout,
                  VkFormat albedoMapFormat,
                  VkImageLayout albedoMapLayout,
                  VkFormat specularMapFormat,
                  VkImageLayout specularMapLayout,
                  VkFormat frontMapFormat,
                  VkImageLayout frontMapLayout,
                  VkFormat backMapFormat,
                  VkImageLayout backMapLayout,
                  LogicalDevice& device);
    PeelingGPass(const PeelingGPass&) = delete;
    PeelingGPass& operator = (const PeelingGPass&) = delete;
  protected:
    virtual ~PeelingGPass() noexcept = default;

  public:
    inline VkFormat depthMapFormat() const noexcept;
    inline VkImageLayout depthMapLayout() const noexcept;
    inline VkFormat normalMapFormat() const noexcept;
    inline VkImageLayout normalMapLayout() const noexcept;
    inline VkFormat albedoMapFormat() const noexcept;
    inline VkImageLayout albedoMapLayout() const noexcept;
    inline VkFormat specularMapFormat() const noexcept;
    inline VkImageLayout specularMapLayout() const noexcept;
    inline VkFormat frontMapFormat() const noexcept;
    inline VkImageLayout frontMapLayout() const noexcept;
    inline VkFormat backMapFormat() const noexcept;
    inline VkImageLayout backMapLayout() const noexcept;

    Ref<FrameBuffer> createFrameBuffer( ImageView& depthMap,
                                        ImageView& normalMap,
                                        ImageView& albedoMap,
                                        ImageView& specularMap,
                                        ImageView& frontMap,
                                        ImageView& backMap);
  private:
    VkFormat _depthMapFormat;
    VkImageLayout _depthMapLayout;
    VkFormat _normalMapFormat;
    VkImageLayout _normalMapLayout;
    VkFormat _albedoMapFormat;
    VkImageLayout _albedoMapLayout;
    VkFormat _specularMapFormat;
    VkImageLayout _specularMapLayout;
    VkFormat _frontMapFormat;
    VkImageLayout _frontMapLayout;
    VkFormat _backMapFormat;
    VkImageLayout _backMapLayout;
  };

  inline VkFormat PeelingGPass::depthMapFormat() const noexcept
  {
    return _depthMapFormat;
  }

  inline VkImageLayout PeelingGPass::depthMapLayout() const noexcept
  {
    return _depthMapLayout;
  }

  inline VkFormat PeelingGPass::normalMapFormat() const noexcept
  {
    return _normalMapFormat;
  }

  inline VkImageLayout PeelingGPass::normalMapLayout() const noexcept
  {
    return _normalMapLayout;
  }

  inline VkFormat PeelingGPass::albedoMapFormat() const noexcept
  {
    return _albedoMapFormat;
  }

  inline VkImageLayout PeelingGPass::albedoMapLayout() const noexcept
  {
    return _albedoMapLayout;
  }

  inline VkFormat PeelingGPass::specularMapFormat() const noexcept
  {
    return _specularMapFormat;
  }

  inline VkImageLayout PeelingGPass::specularMapLayout() const noexcept
  {
    return _specularMapLayout;
  }

  inline VkFormat PeelingGPass::frontMapFormat() const noexcept
  {
    return _frontMapFormat;
  }

  inline VkImageLayout PeelingGPass::frontMapLayout() const noexcept
  {
    return _frontMapLayout;
  }

  inline VkFormat PeelingGPass::backMapFormat() const noexcept
  {
    return _backMapFormat;
  }

  inline VkImageLayout PeelingGPass::backMapLayout() const noexcept
  {
    return _backMapLayout;
  }
}