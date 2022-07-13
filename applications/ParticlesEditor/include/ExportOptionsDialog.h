#pragma once

#include <memory>

#include <QtWidgets/QDialog>

#include <AsyncTasks/EffectExportTask.h>

namespace Ui
{
  class ExportOptionsDialog;
}

class ExportOptionsDialog : public QDialog
{
public:
  explicit ExportOptionsDialog(QWidget* parent);
  ExportOptionsDialog(const ExportOptionsDialog&) = delete;
  ExportOptionsDialog& operator = (const ExportOptionsDialog&) = delete;
  virtual ~ExportOptionsDialog() noexcept;

  EffectExportTask::ExportOptions getOptions() noexcept;

private:
  std::unique_ptr<Ui::ExportOptionsDialog> _ui;
};