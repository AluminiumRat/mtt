#pragma once

#include <memory>

#include <mtt/DrawImpl/Lighting/DirectLightApplicator.h>
#include <mtt/DrawImpl/Lighting/ShadowMapProvider.h>
#include <mtt/Render/Mesh/Mesh.h>
#include <mtt/Render/SceneGraph/SimpleDrawableNode.h>

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

protected:
  virtual void updateBounding() override;
  virtual mtt::Sphere getBoundingSphere() const noexcept override;

  virtual void updateDistance() noexcept override;

private:
  void _updateRadius() noexcept;
  void _updateCylinderMesh() noexcept;
  void _updateDepthCamera() noexcept;
  void _updateShadowsEnabled() noexcept;
  void _updateShadowMapSize() noexcept;
  void _updateCascadeSize() noexcept;
  void _updateBlur() noexcept;

private:
  DirectLightObject& _lightObject;

  std::unique_ptr<mtt::ShadowMapProvider> _shadowMapProvider;

  mtt::DirectLightApplicator _lightDrawable;

  mtt::Mesh _cylinderMesh;
  mtt::SimpleDrawableNode _cylinderNode;

  IconDrawableNode _iconNode;
};