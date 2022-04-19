#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/IntSpinConnection.h>

#include <Objects/ParticlesEmissionAction.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class ParticlesEmissionActionWidget;
}

class ParticlesEmissionActionWidget : public QWidget
{
public:
  ParticlesEmissionActionWidget(ParticlesEmissionAction& object,
                                mtt::Object& targetSelectArea,
                                mtt::UndoStack& undoStack);
  ParticlesEmissionActionWidget(const ParticlesEmissionActionWidget&) = delete;
  ParticlesEmissionActionWidget& operator = (
                                const ParticlesEmissionActionWidget&) = delete;
  virtual ~ParticlesEmissionActionWidget() noexcept;

private:
  std::unique_ptr<Ui::ParticlesEmissionActionWidget> _ui;

  using UintConnection = mtt::IntSpinConnection<ParticlesEmissionAction, uint>;
  std::optional<UintConnection> _particlesNumberConnection;
};