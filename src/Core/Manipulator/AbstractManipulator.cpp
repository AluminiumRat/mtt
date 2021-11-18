#include <mtt/Core/Manipulator/AbstractManipulator.h>

using namespace mtt;

AbstractManipulator::AbstractManipulator() :
  _highlighted(false),
  _activated(false),
  _state(UNSELECTED)
{
}

void AbstractManipulator::release() noexcept
{
  setActivated(false);
}

void AbstractManipulator::setHighlighted(bool newValue) noexcept
{
  _highlighted = newValue;
  _updateState();
}

void AbstractManipulator::setActivated(bool newValue) noexcept
{
  _activated = newValue;
  _updateState();
}

void AbstractManipulator::_updateState() noexcept
{
  State newState = UNSELECTED;
  if(_activated) newState = ACTIVATED;
  else if(_highlighted) newState = HIGHLITED;

  if(newState == _state) return;

  _state = newState;
  processNewState(_state);
}
