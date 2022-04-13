#pragma once

#include <QtWidgets/QMainWindow>

#include <mtt/application/Widgets/AsyncTaskDialog/AsyncTaskDialog.h>

namespace mtt
{
  class EditorMainWindow : public QMainWindow
  {
    Q_OBJECT

  public:
    EditorMainWindow();
    EditorMainWindow(const EditorMainWindow&) = delete;
    EditorMainWindow& operator = (const EditorMainWindow&) = delete;
    virtual ~EditorMainWindow() noexcept = default;

  private:
    void _showAsyncTaskDialog(mtt::AbstractAsyncTask& task);
    void _checkAsyncTaskDialog();
    void _processWarning( mtt::AbstractAsyncTask& task,
                          const QString& message) noexcept;
    void _processError( mtt::AbstractAsyncTask& task,
                        const std::exception& error) noexcept;

  private:
    mtt::AsyncTaskDialog _asyncTaskDialog;
  };
}