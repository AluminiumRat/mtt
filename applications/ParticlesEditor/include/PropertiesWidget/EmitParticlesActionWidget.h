#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/IntSpinConnection.h>

#include <Objects/EmitParticlesAction.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class EmitParticlesActionWidget;
}

class EmitParticlesActionWidget : public QWidget
{
public:
  EmitParticlesActionWidget(EmitParticlesAction& object,
                            mtt::Object& targetSelectArea,
                            mtt::UndoStack& undoStack);
  EmitParticlesActionWidget(const EmitParticlesActionWidget&) = delete;
  EmitParticlesActionWidget& operator = (
                                    const EmitParticlesActionWidget&) = delete;
  virtual ~EmitParticlesActionWidget() noexcept;

private:
  std::unique_ptr<Ui::EmitParticlesActionWidget> _ui;

  using UintConnection = mtt::IntSpinConnection<EmitParticlesAction, uint>;
  std::optional<UintConnection> _particlesNumberConnection;
};