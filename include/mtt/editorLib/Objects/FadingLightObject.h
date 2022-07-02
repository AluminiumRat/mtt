#pragma once

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
    enum FadeType
    {
      NO_FADE = 0,
      LINEAR_FADE = 1,
      INVERSE_LINEAR_FADE = 2,
      IVERSE_SQUARE_FADE = 3
    };
    const static std::map<FadeType, QString> fadeTypeNames;

    Q_PROPERTY(FadeType fadeType
                READ fadeType
                WRITE setFadeType
                RESET resetFadeType
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

    inline FadeType fadeType() const noexcept;
    void setFadeType(FadeType newValue) noexcept;
    inline void resetFadeType() noexcept;

  signals:
    void fadeTypeChanged(FadeType newValue);

  private:
    FadeType _fadeType;
  };

  inline FadingLightObject::FadeType
                                    FadingLightObject::fadeType() const noexcept
  {
    return _fadeType;
  }

  inline void FadingLightObject::resetFadeType() noexcept
  {
    setFadeType(NO_FADE);
  }
}