#pragma once

#include <array>

#include <QtWidgets/QWidget>

namespace Ui
{
  class CubemapWidget;
}

namespace mtt
{
  class CubemapObject;
  class UndoStack;

  class CubemapWidget : public QWidget
  {
    Q_OBJECT

  public:
    CubemapWidget(CubemapObject& object, UndoStack& undoStack);
    CubemapWidget(const CubemapWidget&) = delete;
    CubemapWidget& operator = (const CubemapWidget&) = delete;
    virtual ~CubemapWidget() noexcept;

  private:
    static QString prepareName(const QString& fullName) noexcept;
    void _updateWidget() noexcept;
    void _resetTextures() noexcept;
    void _selectTextures() noexcept;
    void _setTextures(const QStringList& texturesList);

  private:
    Ui::CubemapWidget* _ui;

    CubemapObject& _object;
    UndoStack& _undoStack;
  };
}