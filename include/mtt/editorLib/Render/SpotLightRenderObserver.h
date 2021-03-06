#pragma once

#include <mtt/application/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/clPipeline/Lighting/SpotLight.h>
#include <mtt/editorLib/Render/AbstractLightRenderObserver.h>

namespace mtt
{
  class SpotLightObject;
  class RenderScene;

  class SpotLightRenderObserver : public AbstractLightRenderObserver
  {
    Q_OBJECT

  public:
    SpotLightRenderObserver(SpotLightObject& object,
                            CommonEditData& commonData,
                            RenderScene& renderScene);
    SpotLightRenderObserver(const SpotLightRenderObserver&) = delete;
    SpotLightRenderObserver& operator = (
                                      const SpotLightRenderObserver&) = delete;
    virtual ~SpotLightRenderObserver() noexcept = default;

  private:
    void _updateIlluminance() noexcept;
    void _updateDistance() noexcept;
    void _updateFade() noexcept;
    void _updateAngle() noexcept;
    void _updatePyramidMesh() noexcept;
    void _updateFilterImage() noexcept;
    void _updateShadowsEnabled() noexcept;
    void _updateShadowMapSize() noexcept;
    void _updateBlur() noexcept;

  private:
    SpotLightObject& _lightObject;
    clPipeline::SpotLight _light;
    RenderScene& _renderScene;

    std::unique_ptr<mtt::AsyncTaskQueue::TaskStopper> _filterLoadStopper;
  };
}