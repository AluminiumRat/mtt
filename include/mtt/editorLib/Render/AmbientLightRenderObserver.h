#pragma once

#include <mtt/clPipeline/Lighting/AmbientLight.h>
#include <mtt/editorLib/Render/AbstractLightRenderObserver.h>
#include <mtt/editorLib/Render/CubemapObserver.h>

namespace mtt
{
  class AmbientLightObject;

  class AmbientLightRenderObserver : public AbstractLightRenderObserver
  {
    Q_OBJECT

  public:
    AmbientLightRenderObserver( AmbientLightObject& object,
                                CommonEditData& commonData);
    AmbientLightRenderObserver(const AmbientLightRenderObserver&) = delete;
    AmbientLightRenderObserver& operator = (
                                    const AmbientLightRenderObserver&) = delete;
    virtual ~AmbientLightRenderObserver() noexcept = default;

  private:
    void _updateIlluminance() noexcept;
    void _updateDistance() noexcept;
    void _updateSphereMesh() noexcept;
    void _updateSaturationDistance() noexcept;

  private:
    AmbientLightObject& _lightObject;
    clPipeline::AmbientLight _light;

    CubemapObserver _cubemapObserver;
  };
}