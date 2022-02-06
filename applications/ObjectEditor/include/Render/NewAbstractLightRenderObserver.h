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

class NewAbstractLightRenderObserver : public Object3DRenderObserver
{
  Q_OBJECT

public:
  NewAbstractLightRenderObserver( LightObject& object,
                                  EditorCommonData& commonData);
  NewAbstractLightRenderObserver(
                                const NewAbstractLightRenderObserver&) = delete;
  NewAbstractLightRenderObserver& operator = (
                                const NewAbstractLightRenderObserver&) = delete;
  virtual ~NewAbstractLightRenderObserver() noexcept = default;

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

inline bool NewAbstractLightRenderObserver::infinityArea() const noexcept
{
  return _infinityArea;
}
