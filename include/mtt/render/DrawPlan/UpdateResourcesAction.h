#pragma once

#include <mtt/render/DrawPlan/AbstractAction.h>
#include <mtt/render/DrawPlan/MultipleResourceSetter.h>

namespace mtt
{
  template <typename... ResourceSetterParams>
  class UpdateResourcesAction : public AbstractAction
  {
  public:
    template<typename... SetterArgumentTypes>
    inline UpdateResourcesAction(SetterArgumentTypes&... setterArguments);
    UpdateResourcesAction(const UpdateResourcesAction&) = delete;
    UpdateResourcesAction& operator = (const UpdateResourcesAction&) = delete;
    virtual ~UpdateResourcesAction() noexcept = default;

    inline virtual void execute(DrawContext& context) override;

  private:
    MultipleResourceSetter<ResourceSetterParams...> _resourceSetter;
  };

  template<typename... ResourceSetterParams>
  template<typename... SetterArgumentTypes>
  inline UpdateResourcesAction<ResourceSetterParams...>::
                UpdateResourcesAction(SetterArgumentTypes&... setterArguments) :
    _resourceSetter(setterArguments...)
  {
  }

  template <typename... ResourceSetterParams>
  inline void UpdateResourcesAction<ResourceSetterParams...>::execute(
                                                          DrawContext& context)
  {
    _resourceSetter.setup(context);
  }
}