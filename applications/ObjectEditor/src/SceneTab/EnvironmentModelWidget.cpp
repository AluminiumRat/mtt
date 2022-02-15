#include <mtt/editorLib/Objects/EnvironmentModel.h>

#include <SceneTab/EnvironmentModelWidget.h>
#include <SceneTab/FilenamePropertyWidget.h>

#include <GeneratedFiles/ui_EnvironmentModelWidget.h>

EnvironmentModelWidget::EnvironmentModelWidget( mtt::EnvironmentModel& object,
                                                mtt::UndoStack& undoStack) :
  _ui(new Ui::EnvironmentModelWidget),
  _object(object),
  _undoStack(undoStack)
{
  _ui->setupUi(this);

  using FilenameWidget = FilenamePropertyWidget<mtt::EnvironmentModel>;
  FilenameWidget* filenameWidget =
                      new FilenameWidget(object,
                                        &mtt::EnvironmentModel::filename,
                                        &mtt::EnvironmentModel::setFilename,
                                        &mtt::EnvironmentModel::filenameChanged,
                                        tr("mmd (*.mmd)"),
                                        undoStack);
  _ui->filenameLayout->addWidget(filenameWidget, 3);

  adjustSize();
}

EnvironmentModelWidget::~EnvironmentModelWidget() noexcept = default;