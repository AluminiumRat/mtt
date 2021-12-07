#include <mtt/Application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>

#include <SceneTab/AnimationTrackWidget.h>

#include <GeneratedFiles/ui_AnimationTrackWidget.h>

AnimationTrackWidget::AnimationTrackWidget( AnimationTrack& object,
                                            mtt::Object& skeletonSelectArea,
                                            mtt::UndoStack& undoStack) :
  _ui(new Ui::AnimationTrackWidget)
{
  _ui->setupUi(this);
  _enabledConnection.emplace( *_ui->enabledBox,
                              object,
                              &AnimationTrack::enabled,
                              &AnimationTrack::setEnabled,
                              &AnimationTrack::enabledChanged,
                              undoStack);

  using ReferenceWidget = mtt::ReferencePropertyWidget< AnimationTrack,
                                                        SkeletonObject>;
  ReferenceWidget* referenceWidget = new ReferenceWidget(
                                          object,
                                          &AnimationTrack::skeleton,
                                          &AnimationTrack::setSkeleton,
                                          &AnimationTrack::skeletonRefChanged,
                                          undoStack,
                                          skeletonSelectArea);
  _ui->boneLayout->addWidget(referenceWidget, 3);
  adjustSize();
}

AnimationTrackWidget::~AnimationTrackWidget() noexcept = default;;
