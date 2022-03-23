#pragma once

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>

#include <mtt/utilities/Abort.h>

template <typename ValueType>
class FluidMatrix
{
public:
  inline FluidMatrix(size_t xSize, size_t ySize, size_t zSize);
  FluidMatrix(const FluidMatrix<ValueType>&) = default;
  FluidMatrix<ValueType>& operator = (const FluidMatrix<ValueType>&) = default;
  ~FluidMatrix() noexcept = default;

  inline size_t xSize() const noexcept;
  inline size_t ySize() const noexcept;
  inline size_t zSize() const noexcept;

  inline void clear(const ValueType& newValue) noexcept;

  inline const ValueType& get ( size_t x, size_t y, size_t z) const noexcept;
  inline void set(size_t x,
                  size_t y,
                  size_t z,
                  const ValueType& newValue) noexcept;

  inline ValueType& operator () (size_t x, size_t y, size_t z) noexcept;
  inline const ValueType& operator () ( size_t x,
                                        size_t y,
                                        size_t z) const noexcept;

  inline ValueType interpolate(const glm::vec3& position) const noexcept;
  inline ValueType interpolate(float x, float y, float z) const noexcept;

  inline void swap(FluidMatrix<ValueType>& other) noexcept;

private:
  size_t _xSize;
  size_t _ySize;
  size_t _zSize;
  size_t _xyPlaneSize;

  std::vector<ValueType> _data;
};

template <typename ValueType>
inline FluidMatrix<ValueType>::FluidMatrix( size_t xSize,
                                            size_t ySize,
                                            size_t zSize) :
  _xSize(xSize),
  _ySize(ySize),
  _zSize(zSize),
  _xyPlaneSize(xSize * ySize)
{
  if(_xSize == 0 || _ySize == 0 || _zSize == 0) mtt::Abort("FluidMatrix::FluidMatrix: field size in null");
  _data.resize(_xSize * _ySize * _zSize);
}

template <typename ValueType>
inline size_t FluidMatrix<ValueType>::xSize() const noexcept
{
  return _xSize;
}

template <typename ValueType>
inline size_t FluidMatrix<ValueType>::ySize() const noexcept
{
  return _ySize;
}

template <typename ValueType>
inline size_t FluidMatrix<ValueType>::zSize() const noexcept
{
  return _zSize;
}

template <typename ValueType>
inline void FluidMatrix<ValueType>::clear(const ValueType& newValue) noexcept
{
  std::fill(_data.begin(), _data.end(), newValue);
}

template <typename ValueType>
inline void FluidMatrix<ValueType>::set(size_t x,
                                        size_t y,
                                        size_t z,
                                        const ValueType& newValue) noexcept
{
  size_t position = x + _xSize * y + _xyPlaneSize * z;
  _data[position] = newValue;
}

template <typename ValueType>
inline const ValueType& FluidMatrix<ValueType>::get(size_t x,
                                                    size_t y,
                                                    size_t z) const noexcept
{
  size_t position = x + _xSize * y + _xyPlaneSize * z;
  return _data[position];
}

template <typename ValueType>
inline ValueType& FluidMatrix<ValueType>::operator () ( size_t x,
                                                        size_t y,
                                                        size_t z) noexcept
{
  size_t position = x + _xSize * y + _xyPlaneSize * z;
  return _data[position];
}

template <typename ValueType>
inline const ValueType& FluidMatrix<ValueType>::operator () (
                                                      size_t x,
                                                      size_t y,
                                                      size_t z) const noexcept
{
  return get(x, y, z);
}

template <typename ValueType>
inline ValueType FluidMatrix<ValueType>::interpolate(
                                      const glm::vec3& position) const noexcept
{
  return interpolate(position.x, position.y, position.z);
}

template <typename ValueType>
inline ValueType FluidMatrix<ValueType>::interpolate( float x,
                                                      float y,
                                                      float z) const noexcept
{
  size_t intX = size_t(x);
  float fractX = glm::fract(x);

  size_t intY = size_t(y);
  float fractY = glm::fract(y);

  size_t intZ = size_t(z);
  float fractZ = glm::fract(z);

  ValueType xInter0 = glm::mix( get(intX, intY, intZ),
                                get(intX + 1, intY, intZ),
                                fractX);

  ValueType xInter1 = glm::mix( get(intX, intY + 1, intZ),
                                get(intX + 1, intY + 1, intZ),
                                fractX);

  ValueType xInter2 = glm::mix( get(intX, intY, intZ + 1),
                                get(intX + 1, intY, intZ + 1),
                                fractX);

  ValueType xInter3= glm::mix(get(intX, intY + 1, intZ + 1),
                              get(intX + 1, intY + 1, intZ + 1),
                              fractX);

  ValueType yInter0 = glm::mix(xInter0, xInter1, fractY);
  ValueType yInter1 = glm::mix(xInter2, xInter3, fractY);

  return glm::mix(yInter0, yInter1, fractZ);
}

template <typename ValueType>
inline void FluidMatrix<ValueType>::swap(FluidMatrix<ValueType>& other) noexcept
{
  std::swap(_xSize, other._xSize);
  std::swap(_ySize, other._ySize);
  std::swap(_zSize, other._zSize);
  std::swap(_xyPlaneSize, other._xyPlaneSize);
  std::swap(_data, other._data);
}
