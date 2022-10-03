#pragma once

#include <mtt/clPipeline/Lighting/AmbientLight.h>
#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/render/RenderScene.h>

class RenderScene : public mtt::RenderScene
{
public:
  RenderScene();
  RenderScene(const RenderScene&) = delete;
  RenderScene& operator = (const RenderScene&) = delete;
  virtual ~RenderScene() noexcept;

private:
  mtt::clPipeline::AmbientLight ambientLight;
  mtt::clPipeline::DirectLight directLight;
};
