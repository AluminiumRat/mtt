#pragma once

#include <mtt/application/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/editorLib/Render/HullDrawableNode.h>
#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/particles/Drawable/ParticlesDrawable.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

class ParticleField;

class FieldRenderObserver : public mtt::Object3DRenderObserver
{
  Q_OBJECT

public:
  FieldRenderObserver(ParticleField& object, mtt::CommonEditData& commonData);
  FieldRenderObserver(const FieldRenderObserver&) = delete;
  FieldRenderObserver& operator = (const FieldRenderObserver&) = delete;
  virtual ~FieldRenderObserver() noexcept = default;

private:
  void _updateSize() noexcept;
  void _updateParticles() noexcept;
  void _updateTextures() noexcept;
  void _updateLodMppx() noexcept;
  void _updateLodSmoothing() noexcept;
  void _updateLightingType() noexcept;

private:
  ParticleField& _field;

  mtt::HullDrawableNode _hullNode;

  mtt::ParticlesDrawable _particlesDrawable;
  mtt::SimpleDrawableNode _particlesNode;

  uint8_t _randomTable[256];

  std::unique_ptr<mtt::AsyncTaskQueue::TaskStopper> _uploadTextureStopper;
};