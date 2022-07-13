#include <ExportOptionsDialog.h>

#include <GeneratedFiles/ui_ExportOptionsDialog.h>

ExportOptionsDialog::ExportOptionsDialog(QWidget* parent) :
  QDialog(parent),
  _ui(new Ui::ExportOptionsDialog)
{
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  _ui->setupUi(this);
  setFixedSize(size());
}

ExportOptionsDialog::~ExportOptionsDialog() noexcept = default;

EffectExportTask::ExportOptions ExportOptionsDialog::getOptions() noexcept
{
  EffectExportTask::ExportOptions options;
  options.startTime = mtt::toTimeT(_ui->startTimeSpin->value());
  options.duration = mtt::toTimeT(_ui->durationSpin->value());
  options.timeStep = mtt::toTimeT(_ui->stepSpin->value());
  return options;
}
