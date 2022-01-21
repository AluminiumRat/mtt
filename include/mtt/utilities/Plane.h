#pragma once

#include <glm/glm.hpp>

#include <mtt/utilities/Abort.h>

namespace mtt
{
  class Plane
  {
  public:
    inline Plane() noexcept;
    inline Plane(const glm::vec4& polynomial) noexcept;
    inline Plane(const glm::vec3& point, const glm::vec3& normal);
    Plane(const Plane&) = default;
    Plane& operator = (const Plane&) = default;
    ~Plane() = default;

    inline const glm::vec4& polynomial() const noexcept;
    inline glm::vec3 normal() const noexcept;
    /// Returns the point closest to the origin
    inline glm::vec3 originProjection() const noexcept;
    inline void setPolynomial(const glm::vec4& polynomial) noexcept;
    inline void setPointNormal( const glm::vec3& point,
                                glm::vec3 normal) noexcept;

    inline float signedDistance(const glm::vec3 point) const;

    inline void translate(const glm::mat4& matrix) noexcept;
    inline void fastTranslate(const glm::mat4& inverseTransposeMatrix) noexcept;

  private:
    glm::vec4 _polynomial;
  };

  inline Plane::Plane() noexcept :
    _polynomial(0, 0, 1, 0)
  {
  }

  inline Plane::Plane(const glm::vec4& polynomial) noexcept :
    _polynomial(0, 0, 1, 0)
  {
    setPolynomial(polynomial);
  }

  inline glm::vec3 Plane::normal() const noexcept
  {
    return _polynomial;
  }

  inline glm::vec3 Plane::originProjection() const noexcept
  {
    glm::vec3 toPlaneDirection = -normal();
    return toPlaneDirection * _polynomial.w;
  }

  inline Plane::Plane(const glm::vec3& point, const glm::vec3& normal) :
    _polynomial(0, 0, 1, 0)
  {
    setPointNormal(point, normal);
  }

  inline const glm::vec4& Plane::polynomial() const noexcept
  {
    return _polynomial;
  }

  inline void Plane::setPolynomial(const glm::vec4& polynomial) noexcept
  {
    if( polynomial.x == 0.f &&
        polynomial.y == 0.f &&
        polynomial.z == 0.f) Abort("Plane::setPolynomial: a,b and c is null.");

    glm::vec3 normal(polynomial.x, polynomial.y, polynomial.z);
    _polynomial = polynomial / glm::length(normal);
  }

  inline void Plane::setPointNormal(const glm::vec3& point,
                                    glm::vec3 normal) noexcept
  {
    if( normal.x == 0.f &&
        normal.y == 0.f &&
        normal.z == 0.f) Abort("Plane::setPointNormal: normal is null.");

    normal = glm::normalize(normal);
    float distance = glm::dot(normal, -point);

    _polynomial = glm::vec4(normal, distance);
  }

  inline float Plane::signedDistance(const glm::vec3 point) const
  {
    float inverseNormalDistanceToNull = glm::dot(normal(), point);
    return _polynomial.w + inverseNormalDistanceToNull;
  }

  inline void Plane::translate(const glm::mat4& matrix) noexcept
  {
    fastTranslate(glm::transpose(glm::inverse(matrix)));
  }

  inline void Plane::fastTranslate(
                              const glm::mat4& inverseTransposeMatrix) noexcept
  {
    setPolynomial(inverseTransposeMatrix * _polynomial);
  }
}