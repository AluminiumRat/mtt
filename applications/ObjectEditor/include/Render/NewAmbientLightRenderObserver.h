#pragma once

#include <mtt/clPipeline/Lighting/AmbientLight.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

#include <Render/NewAbstractLightRenderObserver.h>
#include <Render/CubemapObserver.h>
#include <Render/IconDrawableNode.h>

class AmbientLightObject;

class NewAmbientLightRenderObserver : public NewAbstractLightRenderObserver
{
  Q_OBJECT

public:
  NewAmbientLightRenderObserver(AmbientLightObject& object,
                                EditorCommonData& commonData);
  NewAmbientLightRenderObserver(const NewAmbientLightRenderObserver&) = delete;
  NewAmbientLightRenderObserver& operator = (
                                const NewAmbientLightRenderObserver&) = delete;
  virtual ~NewAmbientLightRenderObserver() noexcept = default;

private:
  void _updateIlluminance() noexcept;
  void _updateDistance() noexcept;
  void _updateSphereMesh();
  void _updateSaturationDistance() noexcept;

private:
  AmbientLightObject& _lightObject;
  mtt::clPipeline::AmbientLight _light;

  CubemapObserver _cubemapObserver;

  mtt::Mesh _sphereMesh;
  mtt::SimpleDrawableNode _sphereNode;

  IconDrawableNode _iconNode;
};