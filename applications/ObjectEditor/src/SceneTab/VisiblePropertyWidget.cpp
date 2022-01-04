#include <SceneTab/VisiblePropertyWidget.h>

#include <GeneratedFiles/ui_VisiblePropertyWidget.h>

VisiblePropertyWidget::VisiblePropertyWidget( DisplayedObject& object,
                                              mtt::UndoStack& undoStack) :
  _ui(new Ui::VisiblePropertyWidget)
{
  _ui->setupUi(this);
  _visibleConnection.emplace( *_ui->visibleBox,
                              object,
                              &DisplayedObject::visible,
                              &DisplayedObject::setVisible,
                              &DisplayedObject::visibleChanged,
                              undoStack);
}

VisiblePropertyWidget::~VisiblePropertyWidget() noexcept = default;;
