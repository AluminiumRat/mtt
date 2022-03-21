#include <SceneTab/FluidWidget.h>

#include <GeneratedFiles/ui_FluidWidget.h>

FluidWidget::FluidWidget( FluidObject& object,
                          mtt::UndoStack& undoStack) :
  _ui(new Ui::FluidWidget)
{
  _ui->setupUi(this);

  _cellSizeConnection.emplace(*_ui->cellSizeSpin,
                              object,
                              &FluidObject::cellSize,
                              &FluidObject::setCellSize,
                              &FluidObject::cellSizeChanged,
                              undoStack);
}

FluidWidget::~FluidWidget() noexcept = default;
