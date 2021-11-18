#include <algorithm>
#include <queue>
#include <stdexcept>

#include <mtt/Core/Manipulator/Composite3DManipulator.h>
#include <mtt/Utilities/Log.h>

using namespace mtt;

float Composite3DManipulator::checkTouchPosition(
                                      const TouchInfo& touchInfo) const noexcept
{
  float farthestDistnce = 0;
  for (Manipulator3D* submanipulator : _submanipulators)
  {
    float distance = submanipulator->checkTouchPosition(touchInfo);
    if(distance > farthestDistnce) farthestDistnce = distance;
  }
  return farthestDistnce;
}

std::optional<glm::vec3> Composite3DManipulator::tryActivate(
                                            const TouchInfo& touchInfo) noexcept
{
  struct QueueRecord
  {
    float distance;
    Manipulator3D* manipulator;
  };
  auto cmp = [](const QueueRecord& left, const QueueRecord& right) -> bool
  {
    return left.distance < right.distance;
  };
  using Queue = std::priority_queue<QueueRecord,
                                    std::vector<QueueRecord>,
                                    decltype(cmp)>;
  try
  {
    Queue queue(cmp);

    for (Manipulator3D* submanipulator : _submanipulators)
    {
      float distance = submanipulator->checkTouchPosition(touchInfo);
      if(distance != 0) queue.push({distance, submanipulator});
    }
  
    while(!queue.empty())
    {
      std::optional<glm::vec3> intersecPoint =
                    queue.top().manipulator->tryActivate(touchInfo);
      if(intersecPoint.has_value())
      {
        setActivated(true);
        return intersecPoint;
      }
      queue.pop();
    }
  }
  catch (std::exception& error)
  {
    Log() << "Composite3DManipulator::tryActivate: unable to activate manipulator: " << error.what();
  }
  catch (...)
  {
    Log() << "Composite3DManipulator::tryActivate: unable to activate manipulator";
  }

  return std::nullopt;
}

void Composite3DManipulator::processTouchMove(
                                            const TouchInfo& touchInfo) noexcept
{
  if(highlighted())
  {
    float farthestDistnce = 0;
    Manipulator3D* farthestManipulator = nullptr;
    for (Manipulator3D* submanipulator : _submanipulators)
    {
      float distance = submanipulator->checkTouchPosition(touchInfo);
      if(distance > farthestDistnce)
      {
        farthestDistnce = distance;
        farthestManipulator = submanipulator;
      }
    }
    for (Manipulator3D* submanipulator : _submanipulators)
    {
      try
      {
        submanipulator->setHighlighted(submanipulator == farthestManipulator);
      }
      catch(std::exception& error)
      {
        Log() << "Composite3DManipulator::processMouseMove: unable to highlight a submanipulator: " << error.what();
      }
      catch(...)
      {
        Log() << "Composite3DManipulator::processMouseMove: unable to highlight a submanipulator";
      }
    }
  }
  else
  {
    for (Manipulator3D* submanipulator : _submanipulators)
    {
      try
      {
        submanipulator->setHighlighted(false);
      }
      catch (std::exception& error)
      {
        Log() << "Composite3DManipulator::processMouseMove: unable to unhighlight a submanipulator: " << error.what();
      }
      catch (...)
      {
        Log() << "Composite3DManipulator::processMouseMove: unable to unhighlight a submanipulator";
      }
    }
  }

  for (Manipulator3D* submanipulator : _submanipulators)
  {
    submanipulator->processTouchMove(touchInfo);
  }
}

void Composite3DManipulator::release() noexcept
{
  for(Manipulator3D* submanipulator : _submanipulators)
  {
    if(submanipulator->activated()) submanipulator->release();
  }
  Manipulator3D::release();
}

void Composite3DManipulator::registerSubmanipulator(Manipulator3D& manipulator,
                                                    AbstractNode& joint)
{
  try
  {
    _submanipulators.push_back(&manipulator);
    _drawable.addChild(manipulator.drawable());
    if(!_node.hasChild(joint)) _node.addChild(joint);
  }
  catch(...)
  {
    if(!_submanipulators.empty() && _submanipulators.back() == & manipulator)
    {
      _submanipulators.pop_back();
      _drawable.removeChild(manipulator.drawable());
    }
    throw;
  }
}

AbstractNode& Composite3DManipulator::node() noexcept
{
  return _node;
}

Drawable& Composite3DManipulator::drawable() noexcept
{
  return _drawable;
}

void Composite3DManipulator::processNewState(State newState) noexcept
{
}
