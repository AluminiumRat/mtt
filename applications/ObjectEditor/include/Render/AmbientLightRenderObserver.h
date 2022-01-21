#pragma once

#include <mtt/DLPipeline/Lighting/AmbientLightApplicator.h>
#include <mtt/Render/Mesh/Mesh.h>
#include <mtt/Render/SceneGraph/SimpleDrawableNode.h>

#include <Render/AbstractLightRenderObserver.h>
#include <Render/CubemapObserver.h>
#include <Render/IconDrawableNode.h>

class AmbientLightObject;

class AmbientLightRenderObserver : public AbstractLightRenderObserver
{
  Q_OBJECT

public:
  AmbientLightRenderObserver( AmbientLightObject& object,
                              EditorCommonData& commonData);
  AmbientLightRenderObserver(const AmbientLightRenderObserver&) = delete;
  AmbientLightRenderObserver& operator = (
                                    const AmbientLightRenderObserver&) = delete;
  virtual ~AmbientLightRenderObserver() noexcept = default;

protected:
  virtual void updateBounding() override;
  virtual void updateDistance() noexcept override;

private:
  void _updateSaturationDistance() noexcept;
  void _updateSphereMesh() noexcept;

private:
  AmbientLightObject& _lightObject;

  mtt::DLPipeline::AmbientLightApplicator _lightDrawable;
  CubemapObserver _cubemapObserver;

  mtt::Mesh _sphereMesh;
  mtt::SimpleDrawableNode _sphereNode;

  IconDrawableNode _iconNode;
};