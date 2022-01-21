#include <stdexcept>

#include <mtt/render/RenderPass/AbstractRenderPass.h>
#include <mtt/render/RenderPass/FrameBuffer.h>
#include <mtt/render/LogicalDevice.h>

using namespace mtt;

FrameBuffer::FrameBuffer( AbstractRenderPass& renderPass,
                          const std::vector<ImageView*>& attachments,
                          const std::vector<ImageView*>& samplerMaps) :
  _handle(VK_NULL_HANDLE),
  _device(renderPass.device()),
  _compatabilityIndex(renderPass.compatabilityIndex()),
  _extent(attachments.empty() ? glm::uvec2(0) : attachments[0]->extent())
{
  try
  {
    for (ImageView* image : samplerMaps)
    {
      _samplerMaps.push_back(Ref<ImageView>(image));
    }

    std::vector<VkImageView> images;
    images.reserve(attachments.size());
    for(ImageView* image : attachments)
    {
      if(image->extent().x != _extent.x || image->extent().y != _extent.y) Abort("FrameBuffer::FrameBuffer: frame images have different extent.");
      images.push_back(image->handle());
      _attachments.push_back(Ref<ImageView>(image));
    }

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass.handle();
    framebufferInfo.attachmentCount = uint32_t(images.size());
    framebufferInfo.pAttachments = images.data();
    framebufferInfo.width = _extent.x;
    framebufferInfo.height = _extent.y;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(_device.handle(),
                            &framebufferInfo,
                            nullptr,
                            &_handle) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to create framebuffer.");
    }
  }
  catch(...)
  {
    _cleanup();
    throw;
  }
}

FrameBuffer::~FrameBuffer()
{
  _cleanup();
}

void FrameBuffer::_cleanup() noexcept
{
  if(_handle != VK_NULL_HANDLE)
  {
    vkDestroyFramebuffer(_device.handle(), _handle, nullptr);
    _handle = VK_NULL_HANDLE;
  }
}

bool FrameBuffer::isCompatible(const AbstractRenderPass& pass) const noexcept
{
  if(_compatabilityIndex == 0) return false;
  return _compatabilityIndex == pass.compatabilityIndex();
}
