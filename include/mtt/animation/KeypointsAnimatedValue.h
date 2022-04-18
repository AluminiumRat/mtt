#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <mtt/animation/AbstractAnimatedValue.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/Range.h>

namespace mtt
{
  template <typename KeypointType>
  class KeypointsAnimatedValue : public AbstractAnimatedValue<
                                                  typename KeypointType::ValueT,
                                                  typename KeypointType::TimeT>
  {
  public:
    using KeypointT = typename KeypointType;
    using ValueT = typename KeypointT::ValueT;
    using TimeT = typename KeypointT::TimeT;
    using InterpolatorT = typename KeypointT::InterpolatorT;

  public:
    inline explicit KeypointsAnimatedValue(const ValueT& defaultValue);
    KeypointsAnimatedValue(const KeypointsAnimatedValue&) = delete;
    KeypointsAnimatedValue& operator = (const KeypointsAnimatedValue&) = delete;
    inline virtual ~KeypointsAnimatedValue() noexcept;

    inline size_t keypointsNumber() const noexcept;
    inline KeypointT& keypoint(size_t index) noexcept;
    inline const KeypointT& keypoint(size_t index) const noexcept;
    inline void addKeypoint(std::unique_ptr<KeypointT> keypoint);
    /// Returns removed keypoint
    inline std::unique_ptr<KeypointT> removeKeypoint(
                                            const KeypointT& keypoint) noexcept;
  
    inline virtual ValueT value(TimeT time) const override;

    inline Range<TimeT> timeRange() const noexcept;

  private:
    friend KeypointType;
    void sortKeypoints();

  private:
    ValueT _defaultValue;

    using Keypoints = std::vector<KeypointT*>;
    Keypoints _keypoints;
  };

  template <typename KeypointType>
  inline KeypointsAnimatedValue<KeypointType>::KeypointsAnimatedValue(
                            const typename KeypointType::ValueT& defaultValue) :
    _defaultValue(defaultValue)
  {
  }

  template <typename KeypointType>
  inline KeypointsAnimatedValue<KeypointType>::
                                            ~KeypointsAnimatedValue() noexcept
  {
    for(KeypointType* keypoint : _keypoints) delete keypoint;
  }

  template <typename KeypointType>
  inline size_t KeypointsAnimatedValue<KeypointType>::
                                                keypointsNumber() const noexcept
  {
    return _keypoints.size();
  }

  template <typename KeypointType>
  inline KeypointType& KeypointsAnimatedValue<KeypointType>::
                                                keypoint(size_t index) noexcept
  {
    return *_keypoints[index];
  }

  template <typename KeypointType>
  inline const KeypointType& KeypointsAnimatedValue<KeypointType>::
                                          keypoint(size_t index) const noexcept
  {
    return *_keypoints[index];
  }

  template <typename KeypointType>
  inline void KeypointsAnimatedValue<KeypointType>::addKeypoint(
                                        std::unique_ptr<KeypointType> keypoint)
  {
    typename Keypoints::iterator place = _keypoints.begin();
    while(place != _keypoints.end() &&
          (*place)->time() < keypoint->time())
    {
      place++;
    }

    _keypoints.insert(place, keypoint.get());
    keypoint->setOwnerPtr(this);
    keypoint.release();
  }

  template <typename KeypointType>
  inline std::unique_ptr<KeypointType>
              KeypointsAnimatedValue<KeypointType>::removeKeypoint(
                                          const KeypointType& keypoint) noexcept
  {
    typename Keypoints::iterator iKeypoint = std::find( _keypoints.begin(),
                                                        _keypoints.end(),
                                                        &keypoint);
    if (iKeypoint == _keypoints.end()) Abort("KeypointsAnimatedValue::removeKeypoint: keypoint not found.");

    KeypointType* keypointPtr = *iKeypoint;

    try
    {
      _keypoints.erase(iKeypoint);
    }
    catch(...)
    {
      Abort("KeypointsAnimatedValue::removeKeypoint: Unable to remove keypoint.");
    }

    keypointPtr->setOwnerPtr(nullptr);
    return std::unique_ptr<KeypointType>(keypointPtr);
  }

  template <typename KeypointType>
  void KeypointsAnimatedValue<KeypointType>::sortKeypoints()
  {
    std::sort(_keypoints.begin(),
              _keypoints.end(),
              [](KeypointType* first, KeypointType* second)
              {
                return first->time() < second->time();
              });
  }

  template <typename KeypointType>
  inline typename KeypointType::ValueT
                  KeypointsAnimatedValue<KeypointType>::value(
                                      typename KeypointType::TimeT time) const
  {
    if(_keypoints.empty()) return _defaultValue;
    if(_keypoints.size() == 0) return _keypoints[0]->value();

    if(time <= _keypoints.front()->time()) return _keypoints.front()->value();
    if(time >= _keypoints.back()->time()) return _keypoints.back()->value();

    size_t first = 0;
    size_t second = _keypoints.size();
    while(second - first != 1)
    {
      size_t middle = (first + second) / 2;
      if(keypoint(middle).time() > time) second = middle;
      else first = middle;
    }
  
    return InterpolatorT::calculate(keypoint(first), keypoint(second), time);
  }

  template <typename KeypointType>
  inline Range<typename KeypointType::TimeT>
                KeypointsAnimatedValue<KeypointType>::timeRange() const noexcept
  {
    if (_keypoints.empty()) return Range<TimeT>();
    return Range<TimeT>(_keypoints.front()->time(), _keypoints.back()->time());
  }
};
