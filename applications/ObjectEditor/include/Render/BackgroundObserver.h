#pragma once

#include <memory>
#include <optional>

#include <mtt/clPipeline/Background/Background.h>
#include <mtt/clPipeline/Lighting/AmbientLight.h>
#include <mtt/render/Pipeline/CubeTexture.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>
#include <mtt/render/SceneGraph/AreaModificator.h>

#include <Render/Object3DRenderObserver.h>
#include <Render/CubemapObserver.h>

class BackgroundObject;

class BackgroundObserver : public Object3DRenderObserver
{
  Q_OBJECT

public:
  BackgroundObserver( BackgroundObject& object,
                      EditorCommonData& commonData);
  BackgroundObserver(const BackgroundObserver&) = delete;
  BackgroundObserver& operator = (const BackgroundObserver&) = delete;
  virtual ~BackgroundObserver() noexcept = default;

  inline BackgroundObject& object() const noexcept;

protected:
  virtual void updateVisible(bool newVisible) noexcept override;

private:
  void _updateAreaModificator() noexcept;
  void _updateLuminance() noexcept;
  void _updateDissolutionStartDistance() noexcept;
  void _updateDissolutionLength() noexcept;
  void _updateLight() noexcept;
  void _removeLight() noexcept;
  void _updateLuminanceTexture() noexcept;

private:
  BackgroundObject& _object;

  std::shared_ptr<mtt::CubeTexture> _luminanceTexture;

  mtt::clPipeline::Background _backgroundRenderer;
  mtt::SimpleDrawableNode _backgroundDrawableNode;

  std::optional<mtt::clPipeline::AmbientLight> _light;
  mtt::DrawableNode* _lightDrawable;
  mtt::AreaModificator* _lightModificator;

  CubemapObserver _cubemapObserver;
};

inline BackgroundObject& BackgroundObserver::object() const noexcept
{
  return _object;
}
