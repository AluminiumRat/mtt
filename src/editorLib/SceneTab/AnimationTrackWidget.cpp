#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>
#include <mtt/editorLib/SceneTab/AnimationTrackWidget.h>

#include <GeneratedFiles/ui_AnimationTrackWidget.h>

using namespace mtt;

AnimationTrackWidget::AnimationTrackWidget( AnimationTrack& object,
                                            Object& skeletonSelectArea,
                                            UndoStack& undoStack) :
  _ui(new Ui::AnimationTrackWidget)
{
  _ui->setupUi(this);
  _enabledConnection.emplace( *_ui->enabledBox,
                              object,
                              &AnimationTrack::enabled,
                              &AnimationTrack::setEnabled,
                              &AnimationTrack::enabledChanged,
                              undoStack);

  using ReferenceWidget = ReferencePropertyWidget<AnimationTrack,
                                                  SkeletonObject>;
  ReferenceWidget* referenceWidget =
                        new ReferenceWidget(object,
                                            &AnimationTrack::skeleton,
                                            &AnimationTrack::setSkeleton,
                                            &AnimationTrack::skeletonRefChanged,
                                            undoStack,
                                            skeletonSelectArea);
  _ui->boneLayout->addWidget(referenceWidget, 3);
  adjustSize();
}

AnimationTrackWidget::~AnimationTrackWidget() noexcept = default;;
