#pragma once

#include <optional>

#include <glm/glm.hpp>

namespace mtt
{
  inline std::optional<glm::vec3> lineLineNearest(
                                              glm::vec3 line1Origin,
                                              glm::vec3 line1Direction,
                                              glm::vec3 line2Origin,
                                              glm::vec3 line2Direction) noexcept
  {
    glm::vec3 tangent = glm::cross(line1Direction, line2Direction);
    glm::vec3 planeNormal = glm::cross(tangent, line1Direction);
    if(planeNormal == glm::vec3(0)) return std::nullopt;
    planeNormal = glm::normalize(planeNormal);

    float normCos = glm::dot(planeNormal, line2Direction);
    if(normCos == 0) return std::nullopt;

    float intersectDistance =
                    glm::dot(line1Origin - line2Origin, planeNormal) / normCos;

    return line2Origin + line2Direction * intersectDistance;
  }
}