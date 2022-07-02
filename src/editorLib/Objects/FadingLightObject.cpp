#include <mtt/editorLib/Objects/FadingLightObject.h>

namespace mtt
{
  const std::map<FadingLightObject::FadeType, QString>
    FadingLightObject::fadeTypeNames =
      { {FadingLightObject::NO_FADE, QT_TR_NOOP("no fade")},
        {FadingLightObject::LINEAR_FADE, QT_TR_NOOP("linear")},
        {FadingLightObject::INVERSE_LINEAR_FADE, QT_TR_NOOP("inverse linear")},
        {FadingLightObject::IVERSE_SQUARE_FADE, QT_TR_NOOP("inverse square")}};

  FadingLightObject::FadingLightObject( const QString& name,
                                        bool canBeRenamed,
                                        UID uid) :
    LightWithShadowsObject(name, canBeRenamed, uid),
    _fadeType(NO_FADE)
  {
  }

  void FadingLightObject::setFadeType(FadeType newValue) noexcept
  {
    if(_fadeType == newValue) return;
    _fadeType = newValue;
    emit fadeTypeChanged(_fadeType);
  }
}