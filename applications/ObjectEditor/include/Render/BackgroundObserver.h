#pragma once

#include <memory>
#include <optional>

#include <mtt/clPipeline/Background/Background.h>
#include <mtt/clPipeline/Lighting/AmbientLight.h>
#include <mtt/render/Pipeline/CubeTexture.h>
#include <mtt/render/SceneGraph/AreaModificator.h>

#include <Render/Object3DRenderObserver.h>
#include <Render/CubemapObserver.h>

namespace mtt
{
  class BackgroundObject;
}

class BackgroundObserver : public Object3DRenderObserver
{
  Q_OBJECT

public:
  BackgroundObserver( mtt::BackgroundObject& object,
                      EditorCommonData& commonData);
  BackgroundObserver(const BackgroundObserver&) = delete;
  BackgroundObserver& operator = (const BackgroundObserver&) = delete;
  virtual ~BackgroundObserver() noexcept = default;

  inline mtt::BackgroundObject& object() const noexcept;

protected:
  virtual void updateVisible(bool newVisible) noexcept override;

private:
  void _updateBackgroundVisible() noexcept;
  void _updateLuminance() noexcept;
  void _updateDissolutionStartDistance() noexcept;
  void _updateDissolutionLength() noexcept;
  void _updateLight() noexcept;
  void _removeLight() noexcept;
  void _updateLuminanceTexture() noexcept;

private:
  mtt::BackgroundObject& _object;

  std::shared_ptr<mtt::CubeTexture> _luminanceTexture;

  mtt::clPipeline::Background _backgroundRenderer;

  std::optional<mtt::clPipeline::AmbientLight> _light;

  CubemapObserver _cubemapObserver;
};

inline mtt::BackgroundObject& BackgroundObserver::object() const noexcept
{
  return _object;
}
