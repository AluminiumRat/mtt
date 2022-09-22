#pragma once

#include <QtWidgets/QAction>

namespace mtt
{
  class CommonEditData;

  class DeleteAction : public QAction
  {
    Q_OBJECT

  public:
    /// dialogParentWidget is just parent widget for error dialog. This is not
    /// parent object for QAction.
    DeleteAction( CommonEditData& commonData,
                  QWidget* dialogParentWidget);
    DeleteAction(const DeleteAction&) = delete;
    DeleteAction& operator = (const DeleteAction&) = delete;
    virtual ~DeleteAction() noexcept = default;

  private:
    void _updatEnabled() noexcept;
    void _makeDelete() noexcept;

  private:
    CommonEditData& _commonData;
    QWidget* _dialogParentWidget;
  };
}