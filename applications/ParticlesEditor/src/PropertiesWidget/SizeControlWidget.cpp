#include <PropertiesWidget/SizeControlWidget.h>

#include <GeneratedFiles/ui_SizeControlWidget.h>

SizeControlWidget::SizeControlWidget( SizeControlObject& object,
                                      mtt::UndoStack& undoStack) :
  _ui(new Ui::SizeControlWidget)
{
  _ui->setupUi(this);

  _startSizeConnection.emplace( *_ui->startSizeSpin,
                                object,
                                &SizeControlObject::startSize,
                                &SizeControlObject::setStartSize,
                                &SizeControlObject::startSizeChanged,
                                undoStack);

  _endSizeConnection.emplace( *_ui->endSizeSpin,
                              object,
                              &SizeControlObject::endSize,
                              &SizeControlObject::setEndSize,
                              &SizeControlObject::endSizeChanged,
                              undoStack);
}

SizeControlWidget::~SizeControlWidget() noexcept = default;
