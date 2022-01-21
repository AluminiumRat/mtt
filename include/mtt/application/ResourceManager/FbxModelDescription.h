#pragma once

#include <compare>

#include <QtCore/QString>

#include <mtt/fbx/BaseFbxImporter.h>

namespace mtt
{
  class LogicalDevice;

  struct FbxModelDescription
  {
    QString name = "";
    BaseFbxImporter::MaterialOptions materialOptions =
                                        BaseFbxImporter::blenderMaterialOptions;
    LogicalDevice* device = nullptr;

    std::strong_ordering operator <=> (
                          const FbxModelDescription&) const noexcept = default;
  };
}