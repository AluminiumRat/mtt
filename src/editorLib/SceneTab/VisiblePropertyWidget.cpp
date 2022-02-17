#include <mtt/editorLib/SceneTab/VisiblePropertyWidget.h>

#include <GeneratedFiles/ui_VisiblePropertyWidget.h>

using namespace mtt;

VisiblePropertyWidget::VisiblePropertyWidget( DisplayedObject& object,
                                              UndoStack& undoStack) :
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
