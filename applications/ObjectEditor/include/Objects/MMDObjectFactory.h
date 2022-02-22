#pragma once

#include <mtt/editorLib/Objects/ObjectFactory.h>

class MMDObjectFactory : public mtt::ObjectFactory
{
public:
  static constexpr TypeIndex animationIndex = 5;
  static constexpr TypeIndex animationTrackIndex = 6;
  static constexpr TypeIndex lodIndex = 3;
  static constexpr TypeIndex materialIndex = 1;
  static constexpr TypeIndex meshIndex = 4;
  static constexpr TypeIndex skeletonIndex = 2;

public:
  MMDObjectFactory();
  MMDObjectFactory(const MMDObjectFactory&) = delete;
  MMDObjectFactory& operator = (const MMDObjectFactory&) = delete;
  virtual ~MMDObjectFactory() noexcept = default;
};