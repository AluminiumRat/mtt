#pragma once

#include <algorithm>
#include <limits>
#include <string>
#include <vector>

#include <glm/mat4x4.hpp>

#include <QtCore/QString>

namespace mtt
{
  class TransformTable
  {
  public:
    using Index = size_t;
    static constexpr Index notIndex = std::numeric_limits<Index>::max();

  public:
    TransformTable() = default;
    inline TransformTable(size_t size);
    inline TransformTable(const TransformTable& other);
    inline TransformTable& operator = (const TransformTable& other);
    ~TransformTable() noexcept = default;

    inline size_t size() const noexcept;
    /// returns bone index
    inline Index addBone();
    inline void setTransform( Index boneIndex,
                              const glm::mat4& transform) noexcept;
    inline const glm::mat4& getTransform(Index boneIndex) const noexcept;

    inline void clear() noexcept;

  private:
    std::vector<glm::mat4> _table;
  };

  inline TransformTable::TransformTable(size_t size) :
    _table(size)
  {
  }

  inline TransformTable::TransformTable(const TransformTable& other) :
    _table(other._table)
  {
  }

  inline TransformTable& TransformTable::operator = (
                                                    const TransformTable& other)
  {
    _table = other._table;
    return *this;
  }

  inline size_t TransformTable::size() const noexcept
  {
    return _table.size();
  }

  inline TransformTable::Index TransformTable::addBone()
  {
    _table.push_back(glm::mat4(1));
    return _table.size() - 1;
  }

  inline void TransformTable::setTransform( Index boneIndex,
                                            const glm::mat4& transform) noexcept
  {
    _table[boneIndex] = transform;
  }

  inline const glm::mat4& TransformTable::getTransform(
                                                Index boneIndex) const noexcept
  {
    return _table[boneIndex];
  }

  inline void TransformTable::clear() noexcept
  {
    _table.clear();
  }
}