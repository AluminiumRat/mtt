#pragma once

#include <mtt/render/Drawable/VisibleDrawableFilter.h>

#include <Render/Object3DRenderObserver.h>

namespace mtt
{
  class DrawableNode;
  class AreaModificator;

  namespace clPipeline
  {
    class AbstractLight;
  }
}

class LightObject;

class AbstractLightRenderObserver : public Object3DRenderObserver
{
  Q_OBJECT

public:
  AbstractLightRenderObserver(LightObject& object,
                              EditorCommonData& commonData);
  AbstractLightRenderObserver(const AbstractLightRenderObserver&) = delete;
  AbstractLightRenderObserver& operator = (
                                  const AbstractLightRenderObserver&) = delete;
  virtual ~AbstractLightRenderObserver() noexcept = default;

protected:
  void setLightObject(mtt::clPipeline::AbstractLight& light);

  inline bool infinityArea() const noexcept;
  void setInfinityArea(bool newValue);

private:
  void _clearDrawables() noexcept;
  void _updateEnabled() noexcept;

private:
  LightObject& _object;
  bool _infinityArea;
  mtt::DrawableNode* _defferedLightApplicator;
  mtt::VisibleDrawableFilter _enableFilter;
  mtt::AreaModificator* _forwardLightApplicator;
};

inline bool AbstractLightRenderObserver::infinityArea() const noexcept
{
  return _infinityArea;
}
