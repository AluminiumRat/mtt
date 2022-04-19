#pragma once

#include <mtt/editorLib/Objects/ObjectFactory.h>

class PEEObjectFactory : public mtt::ObjectFactory
{
public:
  static constexpr TypeIndex animationIndex = 1;
  static constexpr TypeIndex positionAnimatorIndex = 2;
  static constexpr TypeIndex emitterIndex = 3;
  static constexpr TypeIndex frameIndex = 4;
  static constexpr TypeIndex particleFieldIndex = 5;
  static constexpr TypeIndex visibilityControlIndex = 6;
  static constexpr TypeIndex gravityModificatorIndex = 7;
  static constexpr TypeIndex blockerObjectIndex = 8;
  static constexpr TypeIndex gasSourceIndex = 9;
  static constexpr TypeIndex heaterObjectIndex = 10;
  static constexpr TypeIndex sizeControlIndex = 11;
  static constexpr TypeIndex ParticlesEmissionActionIndex = 12;
  static constexpr TypeIndex GasEmissionActionIndex = 13;
  static constexpr TypeIndex HeatingActionIndex = 14;

public:
  PEEObjectFactory();
  PEEObjectFactory(const PEEObjectFactory&) = delete;
  PEEObjectFactory& operator = (const PEEObjectFactory&) = delete;
  virtual ~PEEObjectFactory() noexcept = default;
};
