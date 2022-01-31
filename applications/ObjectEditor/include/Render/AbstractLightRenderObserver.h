#pragma once

#include <mtt/clPipeline/Lighting/AbstractLightApplicator.h>
#include <mtt/render/Drawable/VisibleDrawableFilter.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

#include <Render/Object3DRenderObserver.h>

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
  virtual void setApplicator(
                          mtt::clPipeline::AbstractLightApplicator& applicator);
  virtual void updateBounding();
  virtual mtt::Sphere getBoundingSphere() const noexcept;

  virtual void updateDistance() noexcept;

private:
  void _updateEnabled() noexcept;
  void _updateIlluminance() noexcept;

private:
  LightObject& _lightObject;

  mtt::SimpleDrawableNode _applicatorNode;
  mtt::clPipeline::AbstractLightApplicator* _lightApplicator;
  mtt::VisibleDrawableFilter _enableFilter;
};