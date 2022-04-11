#include <PropertiesWidget/LODObjectWidget.h>

#include <GeneratedFiles/ui_LODObjectWidget.h>

LODObjectWidget::LODObjectWidget(LODObject& object, mtt::UndoStack& undoStack) :
  _ui(new Ui::LODObjectWidget)
{
  _ui->setupUi(this);

  _minConnection.emplace( *_ui->minSpin,
                          object,
                          &LODObject::minMppx,
                          &LODObject::setMinMppx,
                          &LODObject::minMppxChanged,
                          undoStack);

  _maxConnection.emplace( *_ui->maxSpin,
                          object,
                          &LODObject::maxMppx,
                          &LODObject::setMaxMppx,
                          &LODObject::maxMppxChanged,
                          undoStack);
}

LODObjectWidget::~LODObjectWidget() noexcept = default;
