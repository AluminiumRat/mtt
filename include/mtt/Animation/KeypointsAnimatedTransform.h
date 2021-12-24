#pragma once

#include <chrono>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <mtt/Animation/AbstractAnimatedTransform.h>
#include <mtt/Animation/CompositeAnimatedTransform.h>
#include <mtt/Animation/KeypointsAnimatedValue.h>
#include <mtt/Animation/ValueKeypoint.h>

namespace mtt
{
  template <typename TimeType>
  class KeypointsAnimatedTransform : public AbstractAnimatedTransform<TimeType>
  {
  public:
    using PositionKeypoint = ValueKeypoint<glm::vec3, TimeType>;
    using PositionAnimation = KeypointsAnimatedValue<PositionKeypoint>;

    using RotationKeypoint = ValueKeypoint<glm::quat, TimeType>;
    using RotationAnimation = KeypointsAnimatedValue<RotationKeypoint>;

    using ScaleKeypoint = ValueKeypoint<glm::vec3, TimeType>;
    using ScaleAnimation = KeypointsAnimatedValue<ScaleKeypoint>;

  public:
    inline KeypointsAnimatedTransform();
    KeypointsAnimatedTransform(const KeypointsAnimatedTransform&) = delete;
    KeypointsAnimatedTransform& operator = (
                                    const KeypointsAnimatedTransform&) = delete;
    virtual ~KeypointsAnimatedTransform() noexcept = default;

    inline const PositionAnimation& positionAnimation() const noexcept;

    inline size_t positionKeypointNumber() const noexcept;
    inline const PositionKeypoint& positionKeypoint(
                                                  size_t index) const noexcept;
    inline void addPositionKeypoint(std::unique_ptr<PositionKeypoint> keypoint);
    /// Returns removed keypoint
    inline std::unique_ptr<PositionKeypoint> removePositionKeypoint(
                                    const PositionKeypoint& keypoint) noexcept;

    inline const RotationAnimation& rotationAnimation() const noexcept;

    inline size_t rotationKeypointNumber() const noexcept;
    inline const RotationKeypoint& rotationKeypoint(
                                                  size_t index) const noexcept;
    inline void addRotationKeypoint(std::unique_ptr<RotationKeypoint> keypoint);
    /// Returns removed keypoint
    inline std::unique_ptr<RotationKeypoint> removeRotationKeypoint(
                                    const RotationKeypoint& keypoint) noexcept;

    inline const ScaleAnimation& scaleAnimation() const noexcept;

    inline size_t scaleKeypointNumber() const noexcept;
    inline const PositionKeypoint& scaleKeypoint(size_t index) const noexcept;
    inline void addScaleKeypoint(std::unique_ptr<ScaleKeypoint> keypoint);
    /// Returns removed keypoint
    inline std::unique_ptr<ScaleKeypoint> removeScaleKeypoint(
                                        const ScaleKeypoint& keypoint) noexcept;

    inline TimeType startTime() const noexcept;
    inline TimeType finishTime() const noexcept;
    inline TimeType duration() const noexcept;

    inline virtual glm::mat4 value(TimeType time) const override;

  protected:
    inline virtual void onStartTimeChanged() noexcept;
    inline virtual void onFinishTimeChanged() noexcept;
    inline virtual void onTimingChanged() noexcept;
    inline virtual void onDurationChanged() noexcept;

  private:
    inline void _updateTiming() noexcept;

  private:
    PositionAnimation _positionAnimation;
    RotationAnimation _rotationAnimation;
    ScaleAnimation _scaleAnimation;

    using CompositeAnimation = CompositeAnimatedTransform<TimeType>;
    CompositeAnimation _compositeAnimation;

    TimeType _startTime;
    TimeType _finishTime;
  };

  template <typename TimeType>
  inline KeypointsAnimatedTransform<TimeType>::KeypointsAnimatedTransform() :
    _positionAnimation(glm::vec3(0.f)),
    _rotationAnimation(glm::quat(1.f, 0.f, 0.f, 0.f)),
    _scaleAnimation(glm::vec3(1.f)),
    _startTime(0),
    _finishTime(0)
  {
    _compositeAnimation.position = &_positionAnimation;
    _compositeAnimation.rotation = &_rotationAnimation;
    _compositeAnimation.scale = &_scaleAnimation;
  }

  template <typename TimeType>
  inline const KeypointsAnimatedTransform<TimeType>::PositionAnimation&
        KeypointsAnimatedTransform<TimeType>::positionAnimation() const noexcept
  {
    return _positionAnimation;
  }

  template <typename TimeType>
  inline size_t KeypointsAnimatedTransform<TimeType>::
                                        positionKeypointNumber() const noexcept
  {
    return _positionAnimation.keypointsNumber();
  }

  template <typename TimeType>
  inline const KeypointsAnimatedTransform<TimeType>::PositionKeypoint&
                  KeypointsAnimatedTransform<TimeType>::positionKeypoint(
                                                    size_t index) const noexcept
  {
    return _positionAnimation.keypoint(index);
  }

  template <typename TimeType>
  inline void KeypointsAnimatedTransform<TimeType>::addPositionKeypoint(
                                    std::unique_ptr<PositionKeypoint> keypoint)
  {
    _positionAnimation.addKeypoint(std::move(keypoint));
    _updateTiming();
  }

  template <typename TimeType>
  inline std::unique_ptr<
    typename KeypointsAnimatedTransform<TimeType>::PositionKeypoint>
      KeypointsAnimatedTransform<TimeType>::removePositionKeypoint(
                                      const PositionKeypoint& keypoint) noexcept
  {
    std::unique_ptr<PositionKeypoint> removedKeypoint =
                                  _positionAnimation.removeKeypoint(keypoint);
    _updateTiming();
    return removedKeypoint;
  }

  template <typename TimeType>
  inline const KeypointsAnimatedTransform<TimeType>::RotationAnimation&
        KeypointsAnimatedTransform<TimeType>::rotationAnimation() const noexcept
  {
    return _rotationAnimation;
  }

  template <typename TimeType>
  inline size_t KeypointsAnimatedTransform<TimeType>::
                                        rotationKeypointNumber() const noexcept
  {
    return _rotationAnimation.keypointsNumber();
  }

  template <typename TimeType>
  inline const KeypointsAnimatedTransform<TimeType>::RotationKeypoint&
                    KeypointsAnimatedTransform<TimeType>::rotationKeypoint(
                                                    size_t index) const noexcept
  {
    return _rotationAnimation.keypoint(index);
  }

  template <typename TimeType>
  inline void KeypointsAnimatedTransform<TimeType>::addRotationKeypoint(
                                    std::unique_ptr<RotationKeypoint> keypoint)
  {
    _rotationAnimation.addKeypoint(std::move(keypoint));
    _updateTiming();
  }

  template <typename TimeType>
  inline std::unique_ptr<
    typename KeypointsAnimatedTransform<TimeType>::RotationKeypoint>
      KeypointsAnimatedTransform<TimeType>::removeRotationKeypoint(
                                      const RotationKeypoint& keypoint) noexcept
  {
    std::unique_ptr<RotationKeypoint> removedKeypoint =
                                    _rotationAnimation.removeKeypoint(keypoint);
    _updateTiming();
    return removedKeypoint;
  }

  template <typename TimeType>
  inline const KeypointsAnimatedTransform<TimeType>::ScaleAnimation&
          KeypointsAnimatedTransform<TimeType>::scaleAnimation() const noexcept
  {
    return _scaleAnimation;
  }

  template <typename TimeType>
  inline size_t KeypointsAnimatedTransform<TimeType>::
                                            scaleKeypointNumber() const noexcept
  {
    return _scaleAnimation.keypointsNumber();
  }

  template <typename TimeType>
  inline const KeypointsAnimatedTransform<TimeType>::PositionKeypoint&
                        KeypointsAnimatedTransform<TimeType>::scaleKeypoint(
                                                    size_t index) const noexcept
  {
    return _scaleAnimation.keypoint(index);
  }

  template <typename TimeType>
  inline void KeypointsAnimatedTransform<TimeType>::addScaleKeypoint(
            std::unique_ptr<KeypointsAnimatedTransform::ScaleKeypoint> keypoint)
  {
    _scaleAnimation.addKeypoint(std::move(keypoint));
    _updateTiming();
  }

  template <typename TimeType>
  inline std::unique_ptr<
    typename KeypointsAnimatedTransform<TimeType>::ScaleKeypoint>
      KeypointsAnimatedTransform<TimeType>::removeScaleKeypoint(
                                        const ScaleKeypoint& keypoint) noexcept
  {
    std::unique_ptr<ScaleKeypoint> removedKeypoint =
                                      _scaleAnimation.removeKeypoint(keypoint);
    _updateTiming();
    return removedKeypoint;
  }

  template <typename TimeType>
  inline TimeType KeypointsAnimatedTransform<TimeType>::
                                                      startTime() const noexcept
  {
    return _startTime;
  }

  template <typename TimeType>
  inline TimeType KeypointsAnimatedTransform<TimeType>::
                                                    finishTime() const noexcept
  {
    return _finishTime;
  }

  template <typename TimeType>
  inline TimeType KeypointsAnimatedTransform<TimeType>::
                                                      duration() const noexcept
  {
    return finishTime() - startTime();
  }

  template <typename TimeType>
  inline glm::mat4 KeypointsAnimatedTransform<TimeType>::
                                                      value(TimeType time) const
  {
    return _compositeAnimation.value(time);
  }

  template <typename TimeType>
  inline void KeypointsAnimatedTransform<TimeType>::_updateTiming() noexcept
  {
    TimeType newStartTime = std::min( _positionAnimation.startTime(),
                                      _rotationAnimation.startTime());
    newStartTime = std::min(newStartTime, _scaleAnimation.startTime());

    TimeType newFinishTime = std::max(_positionAnimation.finishTime(),
                                      _rotationAnimation.finishTime());
    newFinishTime = std::max(newFinishTime, _scaleAnimation.finishTime());

    if (newStartTime == _startTime && newFinishTime == _finishTime) return;

    TimeType oldStartTime = _startTime;
    TimeType oldFinishTime = _finishTime;

    _startTime = newStartTime;
    _finishTime = newFinishTime;

    if (oldStartTime != _startTime) onStartTimeChanged();
    if (oldFinishTime != _finishTime) onFinishTimeChanged();
    onDurationChanged();
    onTimingChanged();
  }

  template <typename TimeType>
  inline void KeypointsAnimatedTransform<TimeType>::
                                                  onStartTimeChanged() noexcept
  {
  }

  template <typename TimeType>
  inline void KeypointsAnimatedTransform<TimeType>::
                                                  onFinishTimeChanged() noexcept
  {
  }

  template <typename TimeType>
  inline void KeypointsAnimatedTransform<TimeType>::onTimingChanged() noexcept
  {
  }

  template <typename TimeType>
  inline void KeypointsAnimatedTransform<TimeType>::onDurationChanged() noexcept
  {
  }
}