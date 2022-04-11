#include <PropertiesWidget/FluidWidget.h>

#include <GeneratedFiles/ui_FluidWidget.h>

FluidWidget::FluidWidget( FluidObject& object,
                          mtt::UndoStack& undoStack) :
  _ui(new Ui::FluidWidget)
{
  _ui->setupUi(this);

  _windConnection.emplace(*_ui->windXSpin,
                          *_ui->windYSpin,
                          *_ui->windZSpin,
                          object,
                          &FluidObject::wind,
                          &FluidObject::setWind,
                          &FluidObject::windChanged,
                          undoStack);

  _cellSizeConnection.emplace(*_ui->cellSizeSpin,
                              object,
                              &FluidObject::cellSize,
                              &FluidObject::setCellSize,
                              &FluidObject::cellSizeChanged,
                              undoStack);

  _iterationsConnection.emplace(*_ui->iterationsSpin,
                                object,
                                &FluidObject::solverIterations,
                                &FluidObject::setSolverIterations,
                                &FluidObject::solverIterationsChanged,
                                undoStack);
}

FluidWidget::~FluidWidget() noexcept = default;
