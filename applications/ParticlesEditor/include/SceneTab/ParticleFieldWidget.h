#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <Objects/ParticleField.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class ParticleFieldWidget;
}

class ParticleFieldWidget : public QWidget
{
public:
  ParticleFieldWidget(ParticleField& object, mtt::UndoStack& undoStack);
  ParticleFieldWidget(const ParticleFieldWidget&) = delete;
  ParticleFieldWidget& operator = (const ParticleFieldWidget&) = delete;
  virtual ~ParticleFieldWidget() noexcept;

private:
  std::unique_ptr<Ui::ParticleFieldWidget> _ui;
};