#pragma once

#include <compare>

#include <QtCore/QString>

namespace mtt
{
  class LogicalDevice;

  struct MMDModelDescription
  {
    QString name = "";
    LogicalDevice* device = nullptr;

    std::strong_ordering operator <=> (
                          const MMDModelDescription&) const noexcept = default;
  };
}