#pragma once

#include <memory>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>

#include <mtt/Animation/AbstractAnimatedTransform.h>

namespace mtt
{
  template <typename TimeType>
  class CompositeAnimatedTransform : public AbstractAnimatedTransform<TimeType>
  {
  public:
    using Position = AbstractAnimatedValue<glm::vec3, TimeType>;
    using Rotation = AbstractAnimatedValue<glm::quat, TimeType>;
    using Scale = AbstractAnimatedValue<float, TimeType>;

  public:
    CompositeAnimatedTransform() = default;
    CompositeAnimatedTransform(const CompositeAnimatedTransform&) = delete;
    CompositeAnimatedTransform& operator = (
                                    const CompositeAnimatedTransform&) = delete;
    virtual ~CompositeAnimatedTransform() = default;

    inline virtual glm::mat4 value(TimeType time) const override;

    std::unique_ptr<Position> position;
    std::unique_ptr<Rotation> rotation;
    std::unique_ptr<Scale> scale;
  };

  template <typename TimeType>
  inline glm::mat4 CompositeAnimatedTransform<TimeType>::value(
                                                            TimeType time) const
  {
    glm::vec3 positionValue = position != nullptr ? position->value(time) :
                                                    glm::vec3(0);

    glm::quat rotationValue = rotation != nullptr ? rotation->value(time) :
                                                    glm::quat();
  
    float scaleValue = scale ? scale->value(time) : 1.f;
  
    return  glm::translate(positionValue) *
            glm::mat4_cast(rotationValue) *
            glm::scale(glm::vec3(scaleValue));
  }
};