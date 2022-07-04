#pragma once

#include <mtt/application/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/clPipeline/Lighting/PointLight.h>
#include <mtt/editorLib/Render/AbstractLightRenderObserver.h>
#include <mtt/editorLib/Render/CubemapObserver.h>

namespace mtt
{
  class PointLightObject;
  class RenderScene;

  class PointLightRenderObserver : public AbstractLightRenderObserver
  {
    Q_OBJECT

  public:
    PointLightRenderObserver( PointLightObject& object,
                              CommonEditData& commonData,
                              RenderScene& renderScene);
    PointLightRenderObserver(const PointLightRenderObserver&) = delete;
    PointLightRenderObserver& operator = (
                                      const PointLightRenderObserver&) = delete;
    virtual ~PointLightRenderObserver() noexcept = default;

  private:
    void _updateIlluminance() noexcept;
    void _updateDistance() noexcept;
    void _updateFade() noexcept;
    void _updateSphereMesh() noexcept;
    void _updateShadowsEnabled() noexcept;
    void _updateShadowMapSize() noexcept;
    void _updateBlur() noexcept;

  private:
    PointLightObject& _lightObject;
    clPipeline::PointLight _light;
    CubemapObserver _cubemapObserver;
    RenderScene& _renderScene;
  };
}