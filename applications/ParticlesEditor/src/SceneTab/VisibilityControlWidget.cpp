#include <SceneTab/VisibilityControlWidget.h>

#include <GeneratedFiles/ui_VisibilityControlWidget.h>

VisibilityControlWidget::VisibilityControlWidget(
                                                VisibilityControlObject& object,
                                                mtt::UndoStack& undoStack) :
  _ui(new Ui::VisibilityControlWidget)
{
  _ui->setupUi(this);

  _saturationConnection.emplace(*_ui->minSaturationSpin,
                                *_ui->maxSaturationSpin,
                                object,
                                &VisibilityControlObject::saturationRange,
                                &VisibilityControlObject::setSaturationRange,
                                &VisibilityControlObject::saturationRangeChanged,
                                undoStack);
  _saturationConnection->setMultiplier(100);
}

VisibilityControlWidget::~VisibilityControlWidget() noexcept = default;
