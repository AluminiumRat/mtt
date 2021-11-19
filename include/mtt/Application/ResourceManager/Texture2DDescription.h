#pragma once

#include <QtCore/QString>

#include <vulkan/vulkan.h>

namespace mtt
{
  class LogicalDevice;

  struct Texture2DDescription
  {
    QString name = "";
    LogicalDevice* device = nullptr;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    bool lods = true;
  };

  inline bool operator == ( const Texture2DDescription& left,
                            const Texture2DDescription& right) noexcept
  {
    return  left.name == right.name &&
            left.device == right.device &&
            left.samples == right.samples &&
            left.lods == right.lods;
  }

  inline bool operator != ( const Texture2DDescription& left,
                            const Texture2DDescription& right) noexcept
  {
    return !(left == right);
  }

  inline bool operator < (const Texture2DDescription& left,
                          const Texture2DDescription& right) noexcept
  {
    if(left.name < right.name) return true;
    if(left.name > right.name) return false;

    if(left.device < right.device) return true;
    if(left.device > right.device) return false;

    if(left.samples < right.samples) return true;
    if(left.samples > right.samples) return false;

    if (left.lods < right.lods) return true;

    return false;
  }

  inline bool operator > (const Texture2DDescription& left,
                          const Texture2DDescription& right) noexcept
  {
    return right < left;
  }

  inline bool operator <= ( const Texture2DDescription& left,
                            const Texture2DDescription& right) noexcept
  {
    return !(left > right);
  }

  inline bool operator >= ( const Texture2DDescription& left,
                            const Texture2DDescription& right) noexcept
  {
    return !(left < right);
  }
}