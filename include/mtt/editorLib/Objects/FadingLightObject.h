#pragma once

#include <mtt/clPipeline/Lighting/LightingFade.h>
#include <mtt/editorLib/Objects/LightWithShadowsObject.h>

namespace mtt
{
  class FadingLightObject : public LightWithShadowsObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitFadingLightObject,
                            visitConstFadingLightObject,
                            LightWithShadowsObject)
  public:
    const static std::map<clPipeline::LightingFade, QString> fadeTypeNames;

    Q_PROPERTY( clPipeline::LightingFade fadeType
                READ fadeType
                WRITE setFadeType
                NOTIFY fadeTypeChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

  public:
    FadingLightObject(const QString& name,
                      bool canBeRenamed,
                      UID = UID());
    FadingLightObject(const FadingLightObject&) = delete;
    FadingLightObject& operator = (const FadingLightObject&) = delete;
    virtual ~FadingLightObject() noexcept = default;

    inline clPipeline::LightingFade fadeType() const noexcept;
    void setFadeType(clPipeline::LightingFade newValue) noexcept;

  signals:
    void fadeTypeChanged(clPipeline::LightingFade newValue);

  private:
    clPipeline::LightingFade _fadeType;
  };

  inline clPipeline::LightingFade
                                    FadingLightObject::fadeType() const noexcept
  {
    return _fadeType;
  }
}