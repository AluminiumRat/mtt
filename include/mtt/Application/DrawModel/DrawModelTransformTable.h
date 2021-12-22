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
    inline Index addBone(const QString& name);
    /// returns the index of the first bone with this name, or notIndex value if
    /// bone is not found
    inline Index findBone(const QString& name) const noexcept;

    inline void setTransform( Index boneIndex,
                              const glm::mat4& transform) noexcept;
    inline const glm::mat4& getTransform(Index boneIndex) const noexcept;

  private:
    struct TableRecord
    {
      QString boneName;
      glm::mat4 transform;
    };
    std::vector<TableRecord> _table;
  };

  inline DrawModelTransformTable::Index DrawModelTransformTable::addBone(
                                                            const QString& name)
  {
    _table.push_back(TableRecord{name, glm::mat4(1)});
    return _table.size() - 1;
  }

  inline DrawModelTransformTable::Index DrawModelTransformTable::findBone(
                                            const QString& name) const noexcept
  {
    std::vector<TableRecord>::const_iterator iBone = std::find_if(
                                          _table.begin(),
                                          _table.end(),
                                          [&](const TableRecord& record) -> bool
                                          {
                                            return record.boneName == name;
                                          });
    if(iBone == _table.end()) return notIndex;
    Index index = iBone - _table.begin();
    return index;
  }

  inline void DrawModelTransformTable::setTransform(
                                            Index boneIndex,
                                            const glm::mat4& transform) noexcept
  {
    _table[boneIndex].transform = transform;
  }

  inline const glm::mat4& DrawModelTransformTable::getTransform(
                                                Index boneIndex) const noexcept
  {
    return _table[boneIndex].transform;
  }
}