#pragma once

#include <vector>

#include <mtt/utilities/UID.h>

namespace mtt
{
  class DrawPlanBuildExtraData
  {
  public:
    /// bool value
    /// If selected is true then special "selected" render technique must be
    ///used.
    static const size_t selectedFlagIndex;

    /// UID value
    /// UID of current rendcered object
    static const size_t objectUIDIndex;

    /// std::vector<glm::mat4> value.
    /// Array of bone matrices for current animation
    static const size_t boneMatricesIndex;

  public:
    DrawPlanBuildExtraData() = default;
    DrawPlanBuildExtraData(const DrawPlanBuildExtraData&) = delete;
    DrawPlanBuildExtraData& operator = (const DrawPlanBuildExtraData&) = delete;
    ~DrawPlanBuildExtraData() noexcept = default;

    static size_t allocateIndex() noexcept;

    inline const void* data(size_t index) const noexcept;
    inline void setData(const void* datatr, size_t index);

  private:
    std::vector<const void*> _pointers;
  };

  inline const void* DrawPlanBuildExtraData::data(size_t index) const noexcept
  {
    if(index >= _pointers.size()) return nullptr;
    return _pointers[index];
  }

  inline void DrawPlanBuildExtraData::setData(const void* dataPtr, size_t index)
  {
    if (index >= _pointers.size()) _pointers.resize(index + 1);
    _pointers[index] = dataPtr;
  }
}