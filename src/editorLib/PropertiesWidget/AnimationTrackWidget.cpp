#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>
#include <mtt/editorLib/PropertiesWidget/AnimationTrackWidget.h>

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
}

AnimationTrackWidget::~AnimationTrackWidget() noexcept = default;;
