#pragma once

#include <memory>

#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/clPipeline/Lighting/CascadeShadowMapProvider.h>
#include <mtt/editorLib/Render/AbstractLightRenderObserver.h>

namespace mtt
{
  class DirectLightObject;
  class RenderScene;

  class DirectLightRenderObserver : public AbstractLightRenderObserver
  {
    Q_OBJECT

  public:
    DirectLightRenderObserver(DirectLightObject& object,
                              CommonEditData& commonData,
                              RenderScene& renderScene);
    DirectLightRenderObserver(const DirectLightRenderObserver&) = delete;
    DirectLightRenderObserver& operator = (
                                    const DirectLightRenderObserver&) = delete;
    virtual ~DirectLightRenderObserver() noexcept = default;

  private:
    void _updateIlluminance() noexcept;
    void _updateDistance() noexcept;
    void _updateRadius() noexcept;
    void _updateCylinderMesh() noexcept;
    void _updateShadowsEnabled() noexcept;
    void _removeShadowmapProvider() noexcept;
    void _updateShadowMapSize() noexcept;
    void _updateCascadeSize() noexcept;
    void _updateBlur() noexcept;

  private:
    DirectLightObject& _lightObject;
    std::unique_ptr<clPipeline::CascadeShadowMapProvider> _shadowMapProvider;
    clPipeline::DirectLight _light;
    RenderScene& _renderScene;
  };
}