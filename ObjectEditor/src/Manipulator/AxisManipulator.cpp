#include <glm/gtc/constants.hpp>

#include <mtt/Application/EditCommands/SetPropertyCommand.h>
#include <mtt/Application/EditCommands/UndoStack.h>
#include <mtt/DrawImpl/MeshTechniques/InstrumentalTechnique.h>
#include <mtt/DrawImpl/constants.h>
#include <mtt/Utilities/Abort.h>

#include <Objects/MovableObject.h>
#include <Manipulator/AxisManipulator.h>

#define SIDES_NUMBER 12
#define CILINDER_RADIUS 5.f
#define CILINDER_HEIGHT 100.f

static void addSide(std::vector<glm::vec3>& positions,
                    float startAngle,
                    float endAngle)
{
  glm::vec2 startXY = glm::vec2(cos(startAngle), sin(startAngle)) *
                                                                CILINDER_RADIUS;
  glm::vec2 endXY = glm::vec2(cos(endAngle), sin(endAngle)) * CILINDER_RADIUS;

  positions.push_back(glm::vec3(startXY, 0));
  positions.push_back(glm::vec3(endXY, CILINDER_HEIGHT));
  positions.push_back(glm::vec3(endXY, 0));

  positions.push_back(glm::vec3(startXY, 0));
  positions.push_back(glm::vec3(startXY, CILINDER_HEIGHT));
  positions.push_back(glm::vec3(endXY, CILINDER_HEIGHT));

  positions.push_back(glm::vec3(0));
  positions.push_back(glm::vec3(startXY, 0));
  positions.push_back(glm::vec3(endXY, 0));

  positions.push_back(glm::vec3(0, 0, CILINDER_HEIGHT));
  positions.push_back(glm::vec3(endXY, CILINDER_HEIGHT));
  positions.push_back(glm::vec3(startXY, CILINDER_HEIGHT));
}

AxisManipulator::AxisManipulator( MovableObject& object,
                                  mtt::UndoStack& undoStack) :
  AxisMove3DManipulator(mtt::AutoscaleDrawableModificator::PIXEL_SCALE_MODE),
  _undoStack(undoStack),
  _object(object)
{
  std::vector<glm::vec3> positions;
  positions.reserve(SIDES_NUMBER * 12);
  
  float angleStep = 2.f * glm::pi<float>() / SIDES_NUMBER;
  float currentAngle = 0;
  for(size_t index = 1; index < SIDES_NUMBER; index++)
  {
    float nextAngle = currentAngle + angleStep;
    addSide(positions, currentAngle, nextAngle);
    currentAngle = nextAngle;
  }
  addSide(positions, currentAngle, 0);

  setGeometry(positions);

  setTechnique( mtt::colorFrameType,
                std::make_unique<mtt::InstrumentalTechnique>(
                                          false,
                                          false,
                                          VK_COMPARE_OP_ALWAYS,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST));
}

std::optional<glm::vec3> AxisManipulator::tryActivate(
                                            const TouchInfo& touchInfo) noexcept
{
  std::optional<glm::vec3> intersecPoint =
                                  AxisMove3DManipulator::tryActivate(touchInfo);

  if(intersecPoint.has_value())
  {
    _startWorldToObject = glm::inverse(_object.coordSystemTransform());
    _startPosition = _object.position();
    try
    {
      _groupCommandLocker.reset(_undoStack);
    }
    catch(...)
    {
      mtt::Abort("AxisManipulator::tryActivate: unable to lock undo group");
    }
  }

  return intersecPoint;
}

void AxisManipulator::processShift( const glm::vec3& startTouchPoint,
                                    const glm::vec3& shiftedPoint) noexcept
{
  glm::vec3 shiftInWorld = shiftedPoint - startTouchPoint;
  glm::vec3 shiftInLocal = _startWorldToObject * glm::vec4(shiftInWorld, 0);
  glm::vec3 newPosition = _startPosition + shiftInLocal;
  if(_object.position() == newPosition) return;

  try
  {
    using Command = mtt::SetPropertyCommand<
                                    MovableObject,
                                    glm::vec3,
                                    void (MovableObject::*)(const glm::vec3&)>;
    std::unique_ptr<Command> command(new Command( _object,
                                                  &MovableObject::setPosition,
                                                  _object.position(),
                                                  newPosition));
    _undoStack.addAndMake(std::move(command));
  }
  catch(std::exception& error)
  {
    mtt::Log() << "AxisManipulator::processShift: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "AxisManipulator::processShift: unknown error";
  }
}

void AxisManipulator::release() noexcept
{
  _groupCommandLocker.reset();
  AxisMove3DManipulator::release();
}
