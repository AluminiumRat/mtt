#include <glm/gtc/constants.hpp>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/clPipeline/MeshTechniques/UnlightedColorTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/editorLib/Manipulator/AxisManipulator.h>
#include <mtt/editorLib/Objects/MovableObject.h>
#include <mtt/utilities/Abort.h>

#define SIDES_NUMBER 12
#define CILINDER_RADIUS 5.f
#define CILINDER_HEIGHT 100.f

using namespace mtt;

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

AxisManipulator::AxisManipulator(MovableObject& object, UndoStack& undoStack) :
  AxisMove3DManipulator(AutoscaleDrawableModificator::PIXEL_SCALE_MODE),
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

  setTechnique( clPipeline::colorFrameType,
                std::make_unique<clPipeline::UnlightedColorTechnique>(
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
      Abort("AxisManipulator::tryActivate: unable to lock undo group");
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
    std::unique_ptr<AbstractEditCommand> command =
                            makeSetPropertyCommand( _object,
                                                    &MovableObject::position,
                                                    &MovableObject::setPosition,
                                                    newPosition);
    _undoStack.addAndMake(std::move(command));
  }
  catch(std::exception& error)
  {
    Log() << "AxisManipulator::processShift: " << error.what();
  }
  catch(...)
  {
    Log() << "AxisManipulator::processShift: unknown error";
  }
}

void AxisManipulator::release() noexcept
{
  _groupCommandLocker.reset();
  AxisMove3DManipulator::release();
}
