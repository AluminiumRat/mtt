#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <mtt/utilities/Plane.h>
#include <mtt/utilities/Sphere.h>

namespace mtt
{
  class ViewFrustum
  {
  public:
    enum Face
    {
      FACE_RIGHT = 0,
      FACE_LEFT = 1,
      FACE_UP = 2,
      FACE_DOWN = 3,
      FACE_NEAR = 4,
      FACE_FAR = 5
    };

  public:
    ViewFrustum() noexcept;
    ViewFrustum(const glm::mat4& inverseViewProjectionMatrix) noexcept;
    ViewFrustum(const ViewFrustum&) = default;
    ViewFrustum& operator = (const ViewFrustum&) = default;
    ~ViewFrustum() = default;

    void setInverseViewProjectionMatrix(const glm::mat4& matrix) noexcept;
  
    inline const Plane& face(Face face) const noexcept;

    inline bool intersect(const Sphere& sphere) const noexcept;

    inline void translate(const glm::mat4& matrix) noexcept;
    inline void fastTranslate(const glm::mat4& inverseTransposeMatrix) noexcept;

  private:
    Plane _faces[6];
  };

  inline const Plane& ViewFrustum::face(Face face) const noexcept
  {
    return _faces[int(face)];
  }

  inline bool ViewFrustum::intersect(const Sphere& sphere) const noexcept
  {
    if(!sphere.valid()) return false;

    for(const Plane& face : _faces)
    {
      if (face.signedDistance(sphere.center) > sphere.radius) return false;
    }

    return true;
  }

  inline void ViewFrustum::translate(const glm::mat4& matrix) noexcept
  {
    glm::mat4 inverseTranspose = glm::transpose(glm::inverse(matrix));
    for (Plane& face : _faces) face.fastTranslate(inverseTranspose);
  }

  inline void ViewFrustum::fastTranslate(
                              const glm::mat4& inverseTransposeMatrix) noexcept
  {
    for (Plane& face : _faces) face.fastTranslate(inverseTransposeMatrix);
  }
}