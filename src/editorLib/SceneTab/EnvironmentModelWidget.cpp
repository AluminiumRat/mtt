#include <mtt/application/Widgets/PropertiesWidgets/FilenamePropertyWidget.h>
#include <mtt/editorLib/Objects/EnvironmentModel.h>
#include <mtt/editorLib/SceneTab/EnvironmentModelWidget.h>

#include <GeneratedFiles/ui_EnvironmentModelWidget.h>

using namespace mtt;

EnvironmentModelWidget::EnvironmentModelWidget( EnvironmentModel& object,
                                                UndoStack& undoStack) :
  _ui(new Ui::EnvironmentModelWidget),
  _object(object),
  _undoStack(undoStack)
{
  _ui->setupUi(this);

  using FilenameWidget = FilenamePropertyWidget<EnvironmentModel>;
  FilenameWidget* filenameWidget =
                          new FilenameWidget(object,
                                            &EnvironmentModel::filename,
                                            &EnvironmentModel::setFilename,
                                            &EnvironmentModel::filenameChanged,
                                            tr("mmd (*.mmd)"),
                                            undoStack);
  _ui->filenameLayout->addWidget(filenameWidget, 3);

  adjustSize();
}

EnvironmentModelWidget::~EnvironmentModelWidget() noexcept = default;