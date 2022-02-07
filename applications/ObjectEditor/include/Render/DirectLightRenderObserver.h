#pragma once

#include <memory>

#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/clPipeline/Lighting/ShadowMapProvider.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

#include <Render/AbstractLightRenderObserver.h>
#include <Render/IconDrawableNode.h>

class DirectLightObject;

class DirectLightRenderObserver : public AbstractLightRenderObserver
{
  Q_OBJECT

public:
  DirectLightRenderObserver(DirectLightObject& object,
                            EditorCommonData& commonData);
  DirectLightRenderObserver(const DirectLightRenderObserver&) = delete;
  DirectLightRenderObserver& operator = (
                                    const DirectLightRenderObserver&) = delete;
  virtual ~DirectLightRenderObserver() noexcept = default;

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