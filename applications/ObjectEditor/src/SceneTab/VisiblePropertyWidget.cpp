#include <SceneTab/VisiblePropertyWidget.h>

#include <GeneratedFiles/ui_VisiblePropertyWidget.h>

VisiblePropertyWidget::VisiblePropertyWidget( mtt::DisplayedObject& object,
                                              mtt::UndoStack& undoStack) :
  _ui(new Ui::VisiblePropertyWidget)
{
  _ui->setupUi(this);
  _visibleConnection.emplace( *_ui->visibleBox,
                              object,
                              &mtt::DisplayedObject::visible,
                              &mtt::DisplayedObject::setVisible,
                              &mtt::DisplayedObject::visibleChanged,
                              undoStack);
}

VisiblePropertyWidget::~VisiblePropertyWidget() noexcept = default;;
