#pragma once

#include <mtt/Render/Pipeline/PipelineResource.h>

#include <mtt/Render/ImageView.h>
#include <mtt/Render/Ref.h>

namespace mtt
{
  class StorageImageResource : public PipelineResource
  {
  public:
    explicit StorageImageResource(PipelineResource::Type type);
    StorageImageResource(const StorageImageResource&) = delete;
    StorageImageResource& operator = (const StorageImageResource&) = delete;
    virtual ~StorageImageResource() noexcept = default;

    virtual void prepareToBind(CommandProducer& drawProducer) override;
    virtual void updateDescriptor(VkDescriptorSet descriptorSet,
                                  uint32_t binding,
                                  CommandProducer& drawProducer,
                                  LogicalDevice& device) override;
    virtual size_t revision() const noexcept override;

    /// Image must be in VK_IMAGE_LAYOUT_GENERAL during pipeline binding
    void setAttachedImageView(ImageView* imageView) noexcept;
    inline ImageView* attachedImageView() const noexcept;
  
  private:
    Ref<ImageView> _imageView;
    size_t _revision;
  };

  inline ImageView* StorageImageResource::attachedImageView() const noexcept
  {
    return _imageView.get();
  }
}