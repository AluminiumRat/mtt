#pragma once

#include <vulkan/vulkan.h>

#include <mtt/render/Pipeline/PipelineResource.h>

namespace mtt
{
  class ImageView;

  class InputAttachment : public PipelineResource
  {
  public:
    explicit InputAttachment(LogicalDevice& device) noexcept;
    InputAttachment(const InputAttachment&) = delete;
    InputAttachment& operator = (const InputAttachment&) = delete;
    virtual ~InputAttachment()  noexcept = default;

    virtual void prepareToBind(CommandProducer& drawProducer) override;
    virtual void updateDescriptor(VkDescriptorSet descriptorSet,
                                  uint32_t binding,
                                  CommandProducer& drawProducer,
                                  LogicalDevice& device) override;

    virtual size_t revision() const noexcept override;

    inline ImageView* attachment() const noexcept;
    inline void setAttachment(ImageView* attachment);

  private:
    LogicalDevice& _device;
    ImageView* _attachment;
  };

  inline ImageView* InputAttachment::attachment() const noexcept
  {
    return _attachment;
  }

  inline void InputAttachment::setAttachment(ImageView* attachment)
  {
    _attachment = attachment;
  }
}