#pragma once

#include <memory>
#include <vector>

#include <mtt/render/SceneRenderer/FrameType.h>

namespace mtt
{
  template<typename Technqiue>
  class TechniquesCollection
  {
  public:
    TechniquesCollection() noexcept = default;
    TechniquesCollection(const TechniquesCollection&) = delete;
    TechniquesCollection& operator = (const TechniquesCollection&) = delete;
    ~TechniquesCollection() noexcept = default;

    inline Technqiue* get(FrameType frameType) noexcept;
    inline const Technqiue* get(FrameType frameType) const noexcept;

    inline void set(std::unique_ptr<Technqiue> technique, FrameType frameType);
    /// Returns the removed technique
    inline std::unique_ptr<Technqiue> remove(FrameType frameType) noexcept;

    /// Delegate should have signature void(const Technqiue&);
    template<typename Delegate>
    inline void pass(Delegate delegate) const;

    /// Delegate should have signature void(Technqiue&) or
    /// void(const Technqiue&);
    template<typename Delegate>
    inline void pass(Delegate delegate);

  private:
    mutable std::vector<std::unique_ptr<Technqiue>> _techniques;
  };

  template<typename Technqiue>
  inline const Technqiue* TechniquesCollection<Technqiue>::get(
                                            FrameType frameType) const noexcept
  {
    size_t index = frameType.index();
    if(_techniques.size() <= index) _techniques.resize(index + 1);
    return _techniques[index].get();
  }

  template<typename Technqiue>
  inline Technqiue* TechniquesCollection<Technqiue>::get(
                                                  FrameType frameType) noexcept
  {
    size_t index = frameType.index();
    if(_techniques.size() <= index) _techniques.resize(index + 1);
    return _techniques[index].get();
  }

  template<typename Technqiue>
  inline void TechniquesCollection<Technqiue>::set(
                                          std::unique_ptr<Technqiue> technique,
                                          FrameType frameType)
  {
    size_t index = frameType.index();
    if (_techniques.size() <= index) _techniques.resize(index + 1);
    _techniques[index] = std::move(technique);
  }

  template<typename Technqiue>
  inline std::unique_ptr<Technqiue>
          TechniquesCollection<Technqiue>::remove(FrameType frameType) noexcept
  {
    size_t index = frameType.index();
    if(_techniques.size() <= index) return nullptr;
    return std::move(_techniques[index]);
  }

  template<typename Technqiue>
  template<typename Delegate>
  inline void TechniquesCollection<Technqiue>::pass(Delegate delegate) const
  {
    for(const std::unique_ptr<Technqiue>& technique : _techniques)
    {
      const Technqiue* techniquePtr = technique.get();
      if(techniquePtr != nullptr) delegate(*techniquePtr);
    }
  }

  template<typename Technqiue>
  template<typename Delegate>
  inline void TechniquesCollection<Technqiue>::pass(Delegate delegate)
  {
    for (const std::unique_ptr<Technqiue>& technique : _techniques)
    {
      Technqiue* techniquePtr = technique.get();
      if (techniquePtr != nullptr) delegate(*techniquePtr);
    }
  }
}