#pragma once

#include <mtt/editorLib/Objects/ObjectFactory.h>

class PEEObjectFactory : public mtt::ObjectFactory
{
public:
  static constexpr TypeIndex animationIndex = 1;
  static constexpr TypeIndex animationTrackIndex = 2;
  static constexpr TypeIndex emitterIndex = 3;
  static constexpr TypeIndex frameIndex = 4;
  static constexpr TypeIndex particleFieldIndex = 5;
  static constexpr TypeIndex visibilityControlIndex = 6;

public:
  PEEObjectFactory();
  PEEObjectFactory(const PEEObjectFactory&) = delete;
  PEEObjectFactory& operator = (const PEEObjectFactory&) = delete;
  virtual ~PEEObjectFactory() noexcept = default;
};
