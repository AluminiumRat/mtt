#pragma once

#include <chrono>

#include <mtt/application/WorkCycle/WorkCycle.h>

class World;

/// Just an action for mtt::Aplication that updates the world in the
/// application's work cycle
class UpdateWorldAction : public mtt::WorkCycle::AbstractAction
{
public:
  static const int actionCategory;
  static constexpr float simulationStep = 0.005f;

public:
  UpdateWorldAction(World& world);
  UpdateWorldAction(const UpdateWorldAction&) = delete;
  UpdateWorldAction& operator = (const UpdateWorldAction&) = delete;
  virtual ~UpdateWorldAction() noexcept = default;

  virtual void preAsycPart() override;
  virtual void asycPart() override;
  virtual void postAsycPart() override;

private:
  World& _world;

  std::optional<std::chrono::system_clock::time_point> _lastSystemTime;
  // Time remaining from the previous cycle
  float _timeRemainder;
};