#include <mtt/render/ImageView.h>
#include <mtt/render/LogicalDevice.h>

using namespace mtt;

ImageView::ImageView( Image& image,
                      VkImageViewType viewType,
                      const VkComponentMapping& components,
                      const VkImageSubresourceRange& subresourceRange) :
  _handle(VK_NULL_HANDLE),
  _image(&image),
  _viewType(viewType),
  _components(components),
  _subresourceRange(subresourceRange)
{
  try
  {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image.handle();
    createInfo.viewType = viewType;
    createInfo.format = image.format();
    createInfo.components = components;
    createInfo.subresourceRange = subresourceRange;

    if (vkCreateImageView(image.device().handle(),
                          &createInfo,
                          nullptr,
                          &_handle) != VK_SUCCESS)
    {
      throw std::runtime_error("ImageView: Failed to create image views.");
    }
  }
  catch(...)
  {
    _cleanup();
    throw;
  }
}

ImageView::~ImageView()
{
  _cleanup();
}

void ImageView::_cleanup() noexcept
{
  if(_handle != VK_NULL_HANDLE)
  {
    vkDestroyImageView( _image->device().handle(),
                        _handle,
                        nullptr);
    _handle = VK_NULL_HANDLE;
  }
}
