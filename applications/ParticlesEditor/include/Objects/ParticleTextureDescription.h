#pragma once

#include <vector>

#include <QtCore/QString>

struct ParticleTextureDescription
{
  QString filename;
  uint8_t extent;

  std::strong_ordering operator <=> (
                    const ParticleTextureDescription&) const noexcept = default;
};
using ParticleTextureDescriptions = std::vector<ParticleTextureDescription>;
