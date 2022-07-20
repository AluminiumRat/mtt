#pragma once

#include <compare>

#include <QtCore/QString>

namespace mtt
{
  class LogicalDevice;

  struct DrawModelDescription
  {
    QString name = "";
    LogicalDevice* device = nullptr;

    std::strong_ordering operator <=> (
                          const DrawModelDescription&) const noexcept = default;
  };
}