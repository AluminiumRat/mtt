#pragma once

#include <array>

#include <QtCore/QString>

#include <vulkan/vulkan.h>

namespace mtt
{
  class LogicalDevice;

  struct CubeTextureDescription
  {
    std::array<QString, 6> sideNames;
    LogicalDevice* device = nullptr;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    bool lods = true;

    auto operator <=> (const CubeTextureDescription&) const noexcept = default;
  };
}