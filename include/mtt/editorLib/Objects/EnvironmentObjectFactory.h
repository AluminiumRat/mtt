#pragma once

#include <mtt/editorLib/Objects/ObjectFactory.h>

namespace mtt
{
  class EnvironmentObjectFactory : public ObjectFactory
  {
  public:
    static constexpr TypeIndex EnvironmentGroupIndex = 1;
    static constexpr TypeIndex BackgroundIndex = 7;
    static constexpr TypeIndex AmbientLightIndex = 8;
    static constexpr TypeIndex DirectLightIndex = 9;
    static constexpr TypeIndex EnvironmentModelIndex = 10;

  public:
    EnvironmentObjectFactory();
    EnvironmentObjectFactory(const EnvironmentObjectFactory&) = delete;
    EnvironmentObjectFactory& operator = (
                                      const EnvironmentObjectFactory&) = delete;
    virtual ~EnvironmentObjectFactory() noexcept = default;
  };
}