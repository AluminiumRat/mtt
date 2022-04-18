#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>
#include <mtt/editorLib/Objects/PositionAnimator.h>
#include <mtt/editorLib/PropertiesWidget/PositionAnimatorWidget.h>

#include <GeneratedFiles/ui_PositionAnimatorWidget.h>

using namespace mtt;

PositionAnimatorWidget::PositionAnimatorWidget( PositionAnimator& object,
                                                Object& skeletonSelectArea,
                                                UndoStack& undoStack) :
  _ui(new Ui::PositionAnimatorWidget)
{
  _ui->setupUi(this);

  using ReferenceWidget = ReferencePropertyWidget<PositionAnimator,
                                                  ScalableObject,
                                                  &PositionAnimator::targetRef>;
  ReferenceWidget* referenceWidget =
                        new ReferenceWidget(object,
                                            &PositionAnimator::targetRefChanged,
                                            undoStack,
                                            skeletonSelectArea);
  _ui->targetLayout->addWidget(referenceWidget, 3);
}

PositionAnimatorWidget::~PositionAnimatorWidget() noexcept = default;;
