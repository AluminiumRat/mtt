#include <mtt/application/Widgets/PropertiesWidgets/FilenamePropertyWidget.h>
#include <mtt/editorLib/Objects/EnvironmentModel.h>
#include <mtt/editorLib/PropertiesWidget/EnvironmentModelWidget.h>
#include <mtt/editorLib/EditorApplication.h>

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
                                            tr("Models(*.mmd *.fbx)"),
                                            undoStack);
  _ui->filenameLayout->addWidget(filenameWidget, 3);

  connect(_ui->reloadButton,
          &QPushButton::pressed,
          this,
          &EnvironmentModelWidget::_reloadModel,
          Qt::DirectConnection);
}

EnvironmentModelWidget::~EnvironmentModelWidget() noexcept = default;

void EnvironmentModelWidget::_reloadModel() noexcept
{
  QString filename = _object.filename();
  if(filename.isEmpty()) return;

  LogicalDevice& device = EditorApplication::instance().displayDevice();
  DrawModelLibrary& library = EditorApplication::instance().drawModelLibrary;
  library.release(filename, device);

  _object.setFilename("");
  _object.setFilename(filename);
}
