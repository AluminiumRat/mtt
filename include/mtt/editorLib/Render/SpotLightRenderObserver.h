#pragma once

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
    void _updateConeMesh() noexcept;

  private:
    SpotLightObject& _lightObject;
    RenderScene& _renderScene;
  };
}