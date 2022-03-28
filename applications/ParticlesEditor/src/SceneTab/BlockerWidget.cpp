#include <SceneTab/BlockerWidget.h>

#include <GeneratedFiles/ui_BlockerWidget.h>

BlockerWidget::BlockerWidget(BlockerObject& object, mtt::UndoStack& undoStack) :
  _ui(new Ui::BlockerWidget)
{
  _ui->setupUi(this);

  _sizeConnection.emplace(*_ui->sizeSpin,
                          object,
                          &BlockerObject::size,
                          &BlockerObject::setSize,
                          &BlockerObject::sizeChanged,
                          undoStack);

  _shapeConnection.emplace( *_ui->shapeCombo,
                            object,
                            &BlockerObject::shape,
                            &BlockerObject::setShape,
                            &BlockerObject::shapeChanged,
                            BlockerObject::shapeNames,
                            undoStack);
}

BlockerWidget::~BlockerWidget() noexcept = default;
