#pragma once

#include <memory>

#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/clPipeline/Lighting/ShadowMapProvider.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

#include <Render/NewAbstractLightRenderObserver.h>
#include <Render/IconDrawableNode.h>

class DirectLightObject;

class NewDirectLightRenderObserver : public NewAbstractLightRenderObserver
{
  Q_OBJECT

public:
  NewDirectLightRenderObserver( DirectLightObject& object,
                                EditorCommonData& commonData);
  NewDirectLightRenderObserver(const NewDirectLightRenderObserver&) = delete;
  NewDirectLightRenderObserver& operator = (
                                  const NewDirectLightRenderObserver&) = delete;
  virtual ~NewDirectLightRenderObserver() noexcept = default;

private:
  void _updateIlluminance() noexcept;
  void _updateDistance() noexcept;
  void _updateRadius() noexcept;
  void _updateCylinderMesh() noexcept;
  void _updateDepthCamera() noexcept;
  void _updateShadowsEnabled() noexcept;
  void _removeShadowmapProvider() noexcept;
  void _updateShadowMapSize() noexcept;
  void _updateCascadeSize() noexcept;
  void _updateBlur() noexcept;

private:
  DirectLightObject& _lightObject;

  std::unique_ptr<mtt::clPipeline::ShadowMapProvider> _shadowMapProvider;

  mtt::clPipeline::DirectLight _light;

  mtt::Mesh _cylinderMesh;
  mtt::SimpleDrawableNode _cylinderNode;

  IconDrawableNode _iconNode;
};