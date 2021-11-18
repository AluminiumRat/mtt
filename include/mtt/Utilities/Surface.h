#pragma once

#include <glm/vec3.hpp>

#include <cstdint>
#include <optional>
#include <vector>

#include <mtt/Utilities/Sphere.h>

namespace mtt
{
  struct Segment;

  class Surface
  {
  public:
    using IndexType = uint16_t;
    using Vertices = std::vector<glm::vec3>;
    using Indices = std::vector<IndexType>;

  public:
    Surface() = default;
    Surface(const Surface&) = default;
    Surface& operator = (const Surface&) = default;
    ~Surface() = default;

    inline const Vertices& vertices() const noexcept;
    void setVertices(const glm::vec3* ptr, size_t number);
    void setVertices(const Vertices& vertices);

    inline const Indices& indices() const noexcept;
    void setIndices(const IndexType* ptr, size_t number);
    void setIndices(const Indices& indices);

    inline const Sphere& bound() const noexcept;

    /// Nearest to segment.p1
    std::optional<glm::vec3> nearestIntersection(
                                          const Segment& segment) const noexcept;

  private:
    void _clearVertices() noexcept;
    void _clearIndices() noexcept;
    void _calculateBound() noexcept;
  
    template<typename Functor>
    void _passTriangles(Functor& functor) const noexcept;

  private:
    Vertices _vertices;
    Indices _indices;

    Sphere _bound;
  };

  inline const Surface::Vertices& Surface::vertices() const noexcept
  {
    return _vertices;
  }

  inline const Surface::Indices& Surface::indices() const noexcept
  {
    return _indices;
  }

  inline const Sphere& Surface::bound() const noexcept
  {
    return _bound;
  }
}