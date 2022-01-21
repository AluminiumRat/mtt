#pragma once

#include <glm/vec3.hpp>

#include <mtt/render/Drawable/Drawable.h>

namespace mtt
{
  namespace dlPipeline
  {
    class AbstractLightApplicator : public Drawable
    {
    public:
      AbstractLightApplicator();
      AbstractLightApplicator(const AbstractLightApplicator&) = delete;
      AbstractLightApplicator& operator = (
                                      const AbstractLightApplicator&) = delete;
      virtual ~AbstractLightApplicator() noexcept = default;
  
      inline float distance() const noexcept;
      inline virtual void setDistance(float newValue);

      /// Illuminance at half of distance in kiloluxes
      inline const glm::vec3& baseIlluminance() const noexcept;
      inline virtual void setIlluminance(const glm::vec3& newValue);

    private:
      float _distance;
      glm::vec3 _baseIlluminance;
    };

    inline float AbstractLightApplicator::distance() const noexcept
    {
      return _distance;
    }

    inline void AbstractLightApplicator::setDistance(float newValue)
    {
      _distance = newValue;
    }

    inline const glm::vec3& AbstractLightApplicator::baseIlluminance()
                                                                  const noexcept
    {
      return _baseIlluminance;
    }

    inline void AbstractLightApplicator::setIlluminance(
                                                      const glm::vec3& newValue)
    {
      _baseIlluminance = newValue;
    }
  }
};