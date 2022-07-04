#include <mtt/editorLib/Objects/FadingLightObject.h>

namespace mtt
{
  const std::map<clPipeline::LightingFade, QString>
    FadingLightObject::fadeTypeNames =
      { {clPipeline::NO_FADE, QT_TR_NOOP("no fade")},
        {clPipeline::LINEAR_FADE, QT_TR_NOOP("linear")},
        {clPipeline::INVERSE_LINEAR_FADE, QT_TR_NOOP("inverse linear")},
        {clPipeline::INVERSE_SQUARE_FADE, QT_TR_NOOP("inverse square")}};

  FadingLightObject::FadingLightObject( const QString& name,
                                        bool canBeRenamed,
                                        UID uid) :
    LightWithShadowsObject(name, canBeRenamed, uid),
    _fadeType(clPipeline::NO_FADE)
  {
  }

  void FadingLightObject::setFadeType(
                                    clPipeline::LightingFade newValue) noexcept
  {
    if(_fadeType == newValue) return;
    _fadeType = newValue;
    emit fadeTypeChanged(_fadeType);
  }
}