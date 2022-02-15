#pragma once

#include <array>

#include <QtWidgets/QWidget>

namespace mtt
{
  class CubemapObject;
  class UndoStack;
}

namespace Ui
{
  class CubemapWidget;
}

class CubemapWidget : public QWidget
{
  Q_OBJECT

public:
  CubemapWidget(mtt::CubemapObject& object, mtt::UndoStack& undoStack);
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

  mtt::CubemapObject& _object;
  mtt::UndoStack& _undoStack;
};
