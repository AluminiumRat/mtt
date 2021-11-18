#pragma once

#include <vector>

#include <glm/vec4.hpp>

namespace mtt
{
  template <size_t dimension, int border>
  inline glm::vec4 clipLine(const glm::vec4& aPoint,
                            const glm::vec4& bPoint) noexcept
  {
    float a = aPoint[dimension];
    float wa = aPoint.w;
    float b = bPoint[dimension];
    float wb = bPoint.w;

    float kA = (border * wb - b) / (a - b + border * (wb - wa));
    float kB = 1.f - kA;

    return aPoint * kA + bPoint * kB;
  }

  template <size_t maxPolySize,
            size_t clipDimension,
            int border,
            int compareSign>
  inline void clipPolyOneDimension( glm::vec4* vertices,
                                    size_t* verticesNumber) noexcept
  {
    glm::vec4 tmpPoly[maxPolySize];
    size_t newVerticesCounter = 0;

    for(size_t sideIndex = 0; sideIndex < *verticesNumber; sideIndex++)
    {
      glm::vec4& startVertex = vertices[sideIndex];
      glm::vec4& endVertex = vertices[(sideIndex + 1) % *verticesNumber];

      float startValue = startVertex[clipDimension];
      float startW = startVertex.w * border;
      float endValue = endVertex[clipDimension];
      float endW = endVertex.w * border;

      bool needWriteStart;
      if constexpr (compareSign == 1) needWriteStart = startValue <= startW;
      else needWriteStart = startValue >= startW;

      if(needWriteStart)
      {
        tmpPoly[newVerticesCounter] = startVertex;
        newVerticesCounter++;
      }

      if( (startValue > startW && endValue < endW) ||
          (startValue < startW && endValue > endW))
      {
        tmpPoly[newVerticesCounter] = clipLine<clipDimension, border>(
                                                                    startVertex,
                                                                    endVertex);
        newVerticesCounter++;
      }
    }

    for(size_t vertexIndex = 0; vertexIndex < newVerticesCounter; vertexIndex++)
    {
      vertices[vertexIndex] = tmpPoly[vertexIndex];
    }
    *verticesNumber = newVerticesCounter;
  }

  template <size_t polygonSize>
  inline std::vector<glm::vec4> clipPolygon(glm::vec4* vertices)
  {
    std::vector<glm::vec4> polygon(polygonSize + 6);
    size_t verticesCount = 0;
    for(; verticesCount < polygonSize; verticesCount++)
    {
      polygon[verticesCount] = vertices[verticesCount];
    }

    clipPolyOneDimension<polygonSize + 6, 0, -1, -1>( polygon.data(),
                                                      &verticesCount);
    clipPolyOneDimension<polygonSize + 6, 0, 1, 1>( polygon.data(),
                                                    &verticesCount);

    clipPolyOneDimension<polygonSize + 6, 1, -1, -1>( polygon.data(),
                                                      &verticesCount);
    clipPolyOneDimension<polygonSize + 6, 1, 1, 1>( polygon.data(),
                                                    &verticesCount);

    clipPolyOneDimension<polygonSize + 6, 2, 0, -1>(polygon.data(),
                                                    &verticesCount);
    clipPolyOneDimension<polygonSize + 6, 2, 1, 1>( polygon.data(),
                                                    &verticesCount);

    polygon.resize(verticesCount);
    return polygon;
  }

  template <size_t clipDimension,
            int border,
            int compareSign>
  inline bool clipSegmentOneDimension(glm::vec4& startPoint,
                                      glm::vec4& endPoint)
  {
    float startValue = startPoint[clipDimension];
    float startW = startPoint.w * border;
    float endValue = endPoint[clipDimension];
    float endW = endPoint.w * border;

    bool discardStart;
    if constexpr (compareSign == 1) discardStart = startValue > startW;
    else discardStart = startValue < startW;

    bool discardEnd;
    if constexpr (compareSign == 1) discardEnd = endValue > endW;
    else discardEnd = endValue < endW;

    if(discardStart & discardEnd) return false;

    if(discardStart || discardEnd)
    {
      glm::vec4 clipPoint = clipLine<clipDimension, border>(startPoint,
                                                            endPoint);
      if(discardStart) startPoint = clipPoint;
      else endPoint = clipPoint;
    }

    return true;
  }

  inline bool clipSegment(glm::vec4& startPoint, glm::vec4& endPoint)
  {
    if(!clipSegmentOneDimension<0, -1, -1>(startPoint, endPoint)) return false;
    if(!clipSegmentOneDimension<0, 1, 1>(startPoint, endPoint)) return false;
    if(!clipSegmentOneDimension<1, -1, -1>(startPoint, endPoint)) return false;
    if(!clipSegmentOneDimension<1, 1, 1>(startPoint, endPoint)) return false;
    if(!clipSegmentOneDimension<2, 0, -1>(startPoint, endPoint)) return false;
    if(!clipSegmentOneDimension<2, 1, 1>(startPoint, endPoint)) return false;
    return true;
  }
}