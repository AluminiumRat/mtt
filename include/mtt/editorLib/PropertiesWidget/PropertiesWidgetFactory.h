#pragma once

#include <mtt/editorLib/Objects/CEVisitor.h>

class QVBoxLayout;

namespace mtt
{
  class CommonEditData;
  class AnimationPlayer;

  class PropertiesWidgetFactory : public CEVisitor
  {
  public:
    PropertiesWidgetFactory(QVBoxLayout& widgetsLayout,
                            CommonEditData& commonEditData,
                            Object* skeletonSelectArea,
                            AnimationPlayer* animationPlayer);
    PropertiesWidgetFactory(const PropertiesWidgetFactory&) = delete;
    PropertiesWidgetFactory& operator = (
                                      const PropertiesWidgetFactory&) = delete;
    virtual ~PropertiesWidgetFactory() noexcept = default;

    inline QVBoxLayout& widgetsLayout() const noexcept;
    inline CommonEditData& commonEditData() const noexcept;
    inline Object* skeletonSelectArea() const noexcept;
    inline AnimationPlayer* animationPlayer() const noexcept;

    virtual void visitAmbientLightObject(AmbientLightObject& object) override;
    virtual void visitAnimationObject(AnimationObject& object) override;
    virtual void visitAnimationTrack(AnimationTrack& object) override;
    virtual void visitBackgroundObject(BackgroundObject& object) override;
    virtual void visitCubemapObject(CubemapObject& object) override;
    virtual void visitDirectLightObject(DirectLightObject& object) override;
    virtual void visitDisplayedObject(DisplayedObject& object) override;
    virtual void visitEnvironmentModel(EnvironmentModel& object) override;
    virtual void visitObject(Object& object) override;
    virtual void visitPositionAnimator(PositionAnimator& object) override;
    virtual void visitFadingLightObject(FadingLightObject& object) override;
    virtual void visitLightObject(LightObject& object) override;
    virtual void visitMovableObject(MovableObject& object) override;
    virtual void visitPointLightObject(PointLightObject& object) override;
    virtual void visitRotatableObject(RotatableObject& object) override;
    virtual void visitScalableObject(ScalableObject& object) override;
    virtual void visitSpotLightObject(SpotLightObject& object) override;

  private:
    QVBoxLayout& _widgetsLayout;
    CommonEditData& _commonEditData;
    Object* _skeletonSelectArea;
    AnimationPlayer* _animationPlayer;
  };

  inline QVBoxLayout& PropertiesWidgetFactory::widgetsLayout() const noexcept
  {
    return _widgetsLayout;
  }

  inline CommonEditData&
                        PropertiesWidgetFactory::commonEditData() const noexcept
  {
    return _commonEditData;
  }

  inline Object* PropertiesWidgetFactory::skeletonSelectArea() const noexcept
  {
    return _skeletonSelectArea;
  }

  inline AnimationPlayer*
                      PropertiesWidgetFactory::animationPlayer() const noexcept
  {
    return _animationPlayer;
  }
}