#pragma once

#include <mtt/clPipeline/Lighting/AmbientLight.h>

#include <Render/AbstractLightRenderObserver.h>
#include <Render/CubemapObserver.h>

namespace mtt
{
  class AmbientLightObject;
}

class AmbientLightRenderObserver : public AbstractLightRenderObserver
{
  Q_OBJECT

public:
  AmbientLightRenderObserver( mtt::AmbientLightObject& object,
                              EditorCommonData& commonData);
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
  mtt::AmbientLightObject& _lightObject;
  mtt::clPipeline::AmbientLight _light;

  CubemapObserver _cubemapObserver;
};