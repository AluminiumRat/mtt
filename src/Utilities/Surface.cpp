#include <glm/gtx/intersect.hpp>

#include <mtt/Utilities/Box.h>
#include <mtt/Utilities/Segment.h>
#include <mtt/Utilities/Surface.h>

using namespace mtt;

void Surface::_clearVertices() noexcept
{
  _vertices.clear();
  _bound.invalidate();
}

void Surface::setVertices(const glm::vec3* ptr, size_t number)
{
  _clearVertices();

  if(ptr != nullptr && number != 0)
  {
    try
    {
      _vertices.reserve(number);
      _vertices.insert(_vertices.end(), ptr, ptr + number);
    }
    catch(...)
    {
      _clearVertices();
      throw;
    }
  }

  _calculateBound();
}

void Surface::setVertices(const Vertices& vertices)
{
  if(vertices.empty()) setVertices(nullptr, 0);
  else setVertices(vertices.data(), vertices.size());
}

void Surface::_clearIndices() noexcept
{
  _indices.clear();
}

void Surface::setIndices(const IndexType* ptr, size_t number)
{
  _clearIndices();
  if(ptr != nullptr && number != 0)
  {
    try
    {
      _indices.reserve(number);
      _indices.insert(_indices.end(), ptr, ptr + number);
    }
    catch (...)
    {
      _clearIndices();
      throw;
    }
  }
}

void Surface::setIndices(const Indices& indices)
{
  if (indices.empty()) setIndices(nullptr, 0);
  else setIndices(indices.data(), indices.size());
}

void Surface::_calculateBound() noexcept
{
  _bound.invalidate();
  if(_vertices.empty()) return;

  Box boundingBox;
  for(const glm::vec3& vertex : _vertices)
  {
    boundingBox.extend(vertex);
  }
  _bound = boundingBox.buildBoundingSphere();
}

template<typename Functor>
void Surface::_passTriangles(Functor& functor) const noexcept
{
  if(_indices.empty())
  {
    size_t totalTriangles = _vertices.size() / 3;
    Vertices::const_iterator vertexCursor = _vertices.begin();
    for(size_t triangleIndex = 0;
        triangleIndex < totalTriangles;
        triangleIndex++)
    {
      const glm::vec3& p1 = *(vertexCursor++);
      const glm::vec3& p2 = *(vertexCursor++);
      const glm::vec3& p3 = *(vertexCursor++);
      functor(p1, p2, p3);
    }
  }
  else
  {
    size_t totalTriangles = _indices.size() / 3;
    Indices::const_iterator indexCursor = _indices.begin();
    for(size_t triangleIndex = 0;
        triangleIndex < totalTriangles;
        triangleIndex++)
    {
      IndexType index1 = *(indexCursor++);
      IndexType index2 = *(indexCursor++);
      IndexType index3 = *(indexCursor++);
      functor(_vertices[index1], _vertices[index2], _vertices[index3]);
    }
  }
}

std::optional<glm::vec3> Surface::nearestIntersection(
                                        const Segment& segment) const noexcept
{
  if(segment.p1 == segment.p2) return std::nullopt;
  if(!_bound.valid()) return std::nullopt;

  glm::vec3 segmentDirection = glm::normalize(segment.p2 - segment.p1);
  float segmentLength = glm::length(segment.p2 - segment.p1);

  glm::vec3 boundIntersect1, normal1, boundIntersect2, normal2;
  if(!glm::intersectLineSphere( segment.p1,
                                segment.p2,
                                _bound.center,
                                _bound.radius,
                                boundIntersect1,
                                normal1,
                                boundIntersect2,
                                normal2)) return std::nullopt;

  float distance1 = glm::length(boundIntersect1 - segment.p1);
  float distance2 = glm::length(boundIntersect2 - segment.p1);
  if( (distance1 < 0 || distance1 > segmentLength) &&
      (distance2 < 0 || distance2 > segmentLength))
  {
    return std::nullopt;
  }

  float nearestDistance = segmentLength;
  std::optional<glm::vec3> result;
  auto functor = [&]( const glm::vec3& p1,
                      const glm::vec3& p2,
                      const glm::vec3& p3)
  {
    glm::vec3 intersectCoord;
    if(glm::intersectLineTriangle(segment.p1,
                                  segmentDirection,
                                  p1,
                                  p2,
                                  p3,
                                  intersectCoord))
    {
      glm::vec3 edge1 = p2 - p1;
      glm::vec3 edge2 = p3 - p1;
      glm::vec3 intersectPoint = p1 + edge1 * intersectCoord.y +
                                                      edge2 * intersectCoord.z;
      glm::vec3 toPoint = intersectPoint - segment.p1;
      float distance = glm::length(toPoint);
      if(distance < 0) return;
      if(distance <= nearestDistance)
      {
        result = intersectPoint;
        nearestDistance = distance;
      }
    }
  };

  _passTriangles(functor);

  return result;
}
