#pragma once

#include <mtt/application/Scene/ObjectFactory.h>

namespace mtt
{
  class EnvironmentObjectFactory : public ObjectFactory
  {
  public:
    static constexpr TypeIndex EnvironmentGroupIndex = 1;
    static constexpr TypeIndex RootObjectIndex = 2;
    static constexpr TypeIndex BackgroundIndex = 7;
    static constexpr TypeIndex AmbientLightIndex = 8;
    static constexpr TypeIndex DirectLightIndex = 9;
    static constexpr TypeIndex EnvironmentModelIndex = 10;
    static constexpr TypeIndex SpotLightIndex = 11;
    static constexpr TypeIndex PointLightIndex = 12;

  public:
    EnvironmentObjectFactory();
    EnvironmentObjectFactory(const EnvironmentObjectFactory&) = delete;
    EnvironmentObjectFactory& operator = (
                                      const EnvironmentObjectFactory&) = delete;
    virtual ~EnvironmentObjectFactory() noexcept = default;
  };
}