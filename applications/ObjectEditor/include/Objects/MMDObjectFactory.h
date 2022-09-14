#pragma once

#include <mtt/application/Scene/ObjectFactory.h>

class MMDObjectFactory : public mtt::ObjectFactory
{
public:
  static constexpr TypeIndex materialIndex = 1;
  static constexpr TypeIndex skeletonIndex = 2;
  static constexpr TypeIndex lodIndex = 3;
  static constexpr TypeIndex meshIndex = 4;
  static constexpr TypeIndex animationIndex = 5;
  static constexpr TypeIndex positionAnimatorIndex = 6;
  static constexpr TypeIndex rootObjectIndex = 7;

public:
  MMDObjectFactory();
  MMDObjectFactory(const MMDObjectFactory&) = delete;
  MMDObjectFactory& operator = (const MMDObjectFactory&) = delete;
  virtual ~MMDObjectFactory() noexcept = default;
};