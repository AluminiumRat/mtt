#pragma once

#include <algorithm>

#include <glm/gtx/norm.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace mtt
{
  struct Sphere
  {
    inline Sphere(const glm::vec3& theCenter,
                  glm::vec3::value_type theRadius) noexcept;
    inline Sphere() noexcept;
    Sphere(const Sphere&) = default;
    Sphere& operator = (const Sphere&) = default;
    ~Sphere() = default;

    inline bool valid() const noexcept;
    inline void invalidate() noexcept;

    inline void extend(const Sphere& other) noexcept;

    inline void translate(const glm::mat4& translationMatrix) noexcept;

    glm::vec3 center;
    glm::vec3::value_type radius;
  };

  inline Sphere::Sphere(const glm::vec3& theCenter,
                                  glm::vec3::value_type theRadius) noexcept:
    center(theCenter),
    radius(theRadius)
  {
  }

  inline Sphere::Sphere() noexcept :
    center(0, 0, 0),
    radius(-1)
  {
  }

  inline bool Sphere::valid() const noexcept
  {
    return radius >= 0;
  }

  inline void Sphere::invalidate() noexcept
  {
    radius = -1;
  }

  inline void Sphere::extend(const Sphere& other) noexcept
  {
    if(!other.valid()) return;
    if(!valid())
    {
      *this = other;
      return;
    }

    if (radius < other.radius)
    {
      Sphere newSphere(other);
      newSphere.extend(*this);
      *this = newSphere;
      return;
    }

    float centersDistance = glm::length(center - other.center);
    if (centersDistance == 0) return;
    if (centersDistance + other.radius < radius) return;

    float radiusesSumm = radius + other.radius;
    float distance = centersDistance - radiusesSumm;
    float halfDistance = distance / 2;
    float meDistanceToNewCenter = other.radius + halfDistance;
    float otherDistanceToNewCenter = radius + halfDistance;

    float blendFactor = (centersDistance - otherDistanceToNewCenter) /
                                                                centersDistance;
    center = center + (other.center - center) * blendFactor;

    radius = radius + other.radius + halfDistance;
  }

  inline void Sphere::translate(const glm::mat4& translationMatrix) noexcept
  {
    if(!valid()) return;

    center = translationMatrix * glm::vec4(center, 1.f);
    glm::vec3 xScaleVector = glm::abs(translationMatrix[0]);
    float maxLen2 = glm::length2(xScaleVector);

    glm::vec3 yScaleVector = glm::abs(translationMatrix[1]);
    maxLen2 = std::max(maxLen2, glm::length2(yScaleVector));

    glm::vec3 zScaleVector = glm::abs(translationMatrix[2]);
    maxLen2 = std::max(maxLen2, glm::length2(zScaleVector));

    glm::vec3 xyScaleVector = (xScaleVector + yScaleVector) / 1.4142f;
    maxLen2 = std::max(maxLen2, glm::length2(xyScaleVector));

    glm::vec3 xzScaleVector = (xScaleVector + zScaleVector) / 1.4142f;
    maxLen2 = std::max(maxLen2, glm::length2(xzScaleVector));

    glm::vec3 yzScaleVector = (yScaleVector + zScaleVector) / 1.4142f;
    maxLen2 = std::max(maxLen2, glm::length2(yzScaleVector));

    glm::vec3 xyzScaleVector = (xScaleVector + yScaleVector + zScaleVector) /
                                                                        1.732f;
    maxLen2 = std::max(maxLen2, glm::length2(xyzScaleVector));

    radius *= sqrt(maxLen2);
  }
}