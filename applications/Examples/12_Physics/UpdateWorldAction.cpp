#include <algorithm>
#include <stdexcept>

#include <mtt/application/TimeT.h>

#include <UpdateWorldAction.h>
#include <World.h>

const int UpdateWorldAction::actionCategory =
                                        mtt::WorkCycle::createActionCategory();

UpdateWorldAction::UpdateWorldAction(World& world) :
  _world(world),
  _timeRemainder(0)
{
}

void UpdateWorldAction::preAsycPart()
{
}

void UpdateWorldAction::asycPart()
{
  // First launch. Just save current time
  if (!_lastSystemTime.has_value())
  {
    _lastSystemTime = std::chrono::system_clock::now();
    return;
  }

  std::chrono::system_clock::time_point now =
                                          std::chrono::system_clock::now();
  std::chrono::system_clock::duration systemDelta = now -
                                                      _lastSystemTime.value();
  _lastSystemTime = now;

  using FloatDuration = std::chrono::duration<float, std::ratio<1, 1>>;
  FloatDuration floatTimeDelta =
                      std::chrono::duration_cast<FloatDuration>(systemDelta);
  float floatDelta = floatTimeDelta.count();
  floatDelta = std::max(floatDelta, 0.f);
  floatDelta = std::min(floatDelta, 0.1f);

  _timeRemainder += floatDelta;

  // Divide the overall time into small chunks
  while (_timeRemainder > simulationStep)
  {
    _world.asyncWork(simulationStep);
    _timeRemainder -= simulationStep;
  }
}

void UpdateWorldAction::postAsycPart()
{
  _world.synchronousWork();
}
