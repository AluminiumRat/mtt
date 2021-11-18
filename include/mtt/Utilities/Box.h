#pragma once

#include <algorithm>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <mtt/Utilities/Sphere.h>

namespace mtt
{
  struct Box
  {
    inline Box() noexcept;
    inline Box(const glm::vec3& MinCorner, const glm::vec3 MaxCorner) noexcept;
    inline Box( float minx,
                float miny,
                float minz,
                float maxx,
                float maxy,
                float maxz)  noexcept;
    Box(const Box&) noexcept = default;
    Box& operator = (const Box&) noexcept = default;
    ~Box() noexcept = default;

    glm::vec3 minCorner;
    glm::vec3 maxCorner;

    inline bool operator == (const Box& other) const noexcept;

    inline bool valid() const noexcept;
    inline void invalidate() noexcept;
    inline bool isContainPoint(const glm::vec3& point) const noexcept;
    inline bool isContainBox(const Box& other) const noexcept;

    inline glm::vec3 size() const noexcept;
    inline float xSize() const noexcept;
    inline float ySize() const noexcept;
    inline float zSize() const noexcept;
    inline glm::vec3 center() const noexcept;

    inline void extend(const glm::vec3& point) noexcept;
    inline void extend(const Box& otherBox) noexcept;

    inline void scaleFromCenter(float scale) noexcept;

    inline Box translated(const glm::mat4& translationMatrix) const noexcept;

    inline Sphere buildBoundingSphere() const noexcept;
    inline bool intersected(const Box& other) const noexcept;

    inline bool operator == (Box& other) const noexcept;
  };

  inline Box::Box() noexcept :
    minCorner(0),
    maxCorner(-1)
  {
  }

  inline Box::Box(const glm::vec3& MinCorner,
                  const glm::vec3 MaxCorner) noexcept :
    minCorner(MinCorner),
    maxCorner(MaxCorner)
  {
  }

  inline Box::Box(float minx,
                  float miny,
                  float minz,
                  float maxx,
                  float maxy,
                  float maxz) noexcept :
    minCorner(minx, miny, minz),
    maxCorner(maxx, maxy, maxz)
  {
  }

  inline bool Box::operator == (const Box& other) const noexcept
  {
    return minCorner == other.minCorner && maxCorner == other.maxCorner;
  }

  inline bool Box::valid() const noexcept
  {
    return  minCorner.x <= maxCorner.x &&
            minCorner.y <= maxCorner.y &&
            minCorner.z <= maxCorner.z;
  }

  inline void Box::invalidate() noexcept
  {
    minCorner = glm::vec3(0);
    maxCorner = glm::vec3(-1);
  }

  inline bool Box::isContainPoint(const glm::vec3& point) const noexcept
  {
    return  point.x >= minCorner.x &&
            point.x <= maxCorner.x &&
            point.y >= minCorner.y &&
            point.y <= maxCorner.y &&
            point.z >= minCorner.z &&
            point.z <= maxCorner.z;
  }

  inline bool Box::isContainBox(const Box& other) const noexcept
  {
    if (!valid() || !other.valid()) return false;

    return  minCorner.x <= other.minCorner.x &&
            minCorner.y <= other.minCorner.y &&
            minCorner.z <= other.minCorner.z &&
            maxCorner.x >= other.maxCorner.x &&
            maxCorner.y >= other.maxCorner.y &&
            maxCorner.z >= other.maxCorner.z;
  }

  inline glm::vec3 Box::size() const noexcept
  {
    return maxCorner - minCorner;
  }

  inline float Box::xSize() const noexcept
  {
    return maxCorner.x - minCorner.x;
  }

  inline float Box::ySize() const noexcept
  {
    return maxCorner.y - minCorner.y;
  }

  inline float Box::zSize() const noexcept
  {
    return maxCorner.z - minCorner.z;
  }

  inline void Box::extend(const glm::vec3& point) noexcept
  {
    if(!valid())
    {
      minCorner = point;
      maxCorner = point;
    }
    else
    {
      minCorner.x = std::min(minCorner.x, point.x);
      minCorner.y = std::min(minCorner.y, point.y);
      minCorner.z = std::min(minCorner.z, point.z);

      maxCorner.x = std::max(maxCorner.x, point.x);
      maxCorner.y = std::max(maxCorner.y, point.y);
      maxCorner.z = std::max(maxCorner.z, point.z);
    }
  }

  inline void Box::extend(const Box& otherBox) noexcept
  {
    if (!otherBox.valid()) return;
    extend(otherBox.minCorner);
    extend(otherBox.maxCorner);
  }

  inline Sphere Box::buildBoundingSphere() const noexcept
  {
    if(!valid()) return Sphere();
    glm::vec3 sizeVector = maxCorner - minCorner;
    return Sphere((minCorner + maxCorner) / 2.f,
                  glm::length(sizeVector) / 2.f);
  }

  inline glm::vec3 Box::center() const noexcept
  {
    return (minCorner + maxCorner) / 2.f;
  }

  inline void Box::scaleFromCenter(float scale) noexcept
  {
    if(!valid()) return;
    glm::vec3 diagonal = maxCorner - minCorner;
    diagonal *= scale;
    glm::vec3 halfDiagonal = diagonal / 2.f;
    glm::vec3 theCenter = center();
    minCorner = theCenter - halfDiagonal;
    maxCorner = theCenter + halfDiagonal;
  }

  inline Box Box::translated(const glm::mat4& translationMatrix) const noexcept
  {
    if(!valid()) return Box();

    Box result;
    result.extend(translationMatrix * glm::vec4(minCorner, 1.f));
    result.extend(translationMatrix * glm::vec4(minCorner.x,
                                                minCorner.y,
                                                maxCorner.z,
                                                1.f));
    result.extend(translationMatrix * glm::vec4(minCorner.x,
                                                maxCorner.y,
                                                minCorner.z,
                                                1.f));
    result.extend(translationMatrix * glm::vec4(minCorner.x,
                                                maxCorner.y,
                                                maxCorner.z,
                                                1.f));
    result.extend(translationMatrix * glm::vec4(maxCorner.x,
                                                minCorner.y,
                                                minCorner.z,
                                                1.f));
    result.extend(translationMatrix * glm::vec4(maxCorner.x,
                                                minCorner.y,
                                                maxCorner.z,
                                                1.f));
    result.extend(translationMatrix * glm::vec4(maxCorner.x,
                                                maxCorner.y,
                                                minCorner.z,
                                                1.f));
    result.extend(translationMatrix * glm::vec4(maxCorner, 1.f));

    return result;
  }

  inline bool Box::intersected(const Box& other) const noexcept
  {
    if (!valid()) return false;
    if (!other.valid()) return false;

    if(maxCorner.x < other.minCorner.x ||
                                  minCorner.x > other.maxCorner.x) return false;
    if(maxCorner.y < other.minCorner.y ||
                                  minCorner.y > other.maxCorner.y) return false;
    if(maxCorner.z < other.minCorner.z ||
                                  minCorner.z > other.maxCorner.z) return false;
    return true;
  }

  inline bool Box::operator == (Box& other) const noexcept
  {
    return minCorner == other.minCorner && maxCorner == other.maxCorner;
  }
}