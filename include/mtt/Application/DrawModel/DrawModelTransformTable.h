#pragma once

#include <algorithm>
#include <limits>
#include <string>
#include <vector>

#include <glm/mat4x4.hpp>

#include <QtCore/QString>

namespace mtt
{
  class DrawModelTransformTable
  {
  public:
    using Index = size_t;
    static constexpr Index notIndex = std::numeric_limits<Index>::max();

  public:
    DrawModelTransformTable() = default;
    DrawModelTransformTable(const DrawModelTransformTable&) = delete;
    DrawModelTransformTable& operator = (
                                      const DrawModelTransformTable&) = delete;
    ~DrawModelTransformTable() noexcept = default;

    /// returns bone index
    inline Index addBone();
    inline void setTransform( Index boneIndex,
                              const glm::mat4& transform) noexcept;
    inline const glm::mat4& getTransform(Index boneIndex) const noexcept;

  private:
    std::vector<glm::mat4> _table;
  };

  inline DrawModelTransformTable::Index DrawModelTransformTable::addBone()
  {
    _table.push_back(glm::mat4(1));
    return _table.size() - 1;
  }

  inline void DrawModelTransformTable::setTransform(
                                            Index boneIndex,
                                            const glm::mat4& transform) noexcept
  {
    _table[boneIndex] = transform;
  }

  inline const glm::mat4& DrawModelTransformTable::getTransform(
                                                Index boneIndex) const noexcept
  {
    return _table[boneIndex];
  }
}