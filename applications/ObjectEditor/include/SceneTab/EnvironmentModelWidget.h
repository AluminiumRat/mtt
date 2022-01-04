#pragma once

#include <memory>

#include <QtWidgets/QWidget>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class EnvironmentModelWidget;
}

class EnvironmentModel;

class EnvironmentModelWidget : public QWidget
{
  Q_OBJECT

public:
  EnvironmentModelWidget(EnvironmentModel& object, mtt::UndoStack& undoStack);
  EnvironmentModelWidget(const EnvironmentModelWidget&) = delete;
  EnvironmentModelWidget& operator = (const EnvironmentModelWidget&) = delete;
  virtual ~EnvironmentModelWidget() noexcept;

private:
  static QString prepareName(const QString& fullName) noexcept;
  void _updateWidget() noexcept;
  void _resetTextures() noexcept;
  void _selectTextures() noexcept;
  void _setTextures(const QStringList& texturesList);

private:
  std::unique_ptr<Ui::EnvironmentModelWidget> _ui;

  EnvironmentModel& _object;
  mtt::UndoStack& _undoStack;
};
