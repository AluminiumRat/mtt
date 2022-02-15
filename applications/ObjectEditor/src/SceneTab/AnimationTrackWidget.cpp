#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>

#include <SceneTab/AnimationTrackWidget.h>

#include <GeneratedFiles/ui_AnimationTrackWidget.h>

AnimationTrackWidget::AnimationTrackWidget( mtt::AnimationTrack& object,
                                            mtt::Object& skeletonSelectArea,
                                            mtt::UndoStack& undoStack) :
  _ui(new Ui::AnimationTrackWidget)
{
  _ui->setupUi(this);
  _enabledConnection.emplace( *_ui->enabledBox,
                              object,
                              &mtt::AnimationTrack::enabled,
                              &mtt::AnimationTrack::setEnabled,
                              &mtt::AnimationTrack::enabledChanged,
                              undoStack);

  using ReferenceWidget = mtt::ReferencePropertyWidget< mtt::AnimationTrack,
                                                        mtt::SkeletonObject>;
  ReferenceWidget* referenceWidget =
                  new ReferenceWidget(object,
                                      &mtt::AnimationTrack::skeleton,
                                      &mtt::AnimationTrack::setSkeleton,
                                      &mtt::AnimationTrack::skeletonRefChanged,
                                      undoStack,
                                      skeletonSelectArea);
  _ui->boneLayout->addWidget(referenceWidget, 3);
  adjustSize();
}

AnimationTrackWidget::~AnimationTrackWidget() noexcept = default;;
