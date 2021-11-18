#pragma once

#include <mtt/Render/DrawPlan/ResourceSetter.h>

namespace mtt
{
  template <typename... Arguments>
  class MultipleResourceSetter;

  template <>
  class MultipleResourceSetter<>
  { 
  public:
    void setup(DrawContext& drawContext)
    {
    }
  };

  template <typename ResourceType,
            typename ValueType,
            typename... NextSetterArgs>
  class MultipleResourceSetter< ResourceType,
                                ValueType,
                                NextSetterArgs...>
  {
  public:
    template <typename... OtherArguments>
    MultipleResourceSetter( ResourceType& resource,
                            const ValueType& value,
                            OtherArguments&... nextArguments) :
      _currentSetter(resource, value),
      _nextSetter(nextArguments...)
    {
    }

    void setup(DrawContext& drawContext)
    {
      _currentSetter.setup(drawContext);
      _nextSetter.setup(drawContext);
    }

  private:
    ResourceSetter<ResourceType, ValueType> _currentSetter;
    MultipleResourceSetter<NextSetterArgs...> _nextSetter;
  };
}