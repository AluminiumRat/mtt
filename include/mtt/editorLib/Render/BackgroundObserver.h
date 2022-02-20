#pragma once

#include <memory>
#include <optional>

#include <mtt/clPipeline/Background/Background.h>
#include <mtt/clPipeline/Lighting/AmbientLight.h>
#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/editorLib/Render/CubemapObserver.h>
#include <mtt/render/Pipeline/CubeTexture.h>
#include <mtt/render/SceneGraph/AreaModificator.h>

namespace mtt
{
  class BackgroundObject;

  class BackgroundObserver : public Object3DRenderObserver
  {
    Q_OBJECT

  public:
    BackgroundObserver( BackgroundObject& object,
                        CommonEditData& commonData);
    BackgroundObserver(const BackgroundObserver&) = delete;
    BackgroundObserver& operator = (const BackgroundObserver&) = delete;
    virtual ~BackgroundObserver() noexcept = default;

    inline BackgroundObject& object() const noexcept;

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
    BackgroundObject& _object;

    std::shared_ptr<CubeTexture> _luminanceTexture;

    clPipeline::Background _backgroundRenderer;

    std::optional<clPipeline::AmbientLight> _light;

    CubemapObserver _cubemapObserver;
  };

  inline BackgroundObject& BackgroundObserver::object() const noexcept
  {
    return _object;
  }
}