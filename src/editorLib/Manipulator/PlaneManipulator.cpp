#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/clPipeline/MeshTechniques/UnlightedColorTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/editorLib/Manipulator/PlaneManipulator.h>
#include <mtt/editorLib/Objects/MovableObject.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

PlaneManipulator::PlaneManipulator( MovableObject& object,
                                    UndoStack& undoStack) :
  PlaneMove3DManipulator(AutoscaleDrawableModificator::PIXEL_SCALE_MODE),
  _undoStack(undoStack),
  _object(object)
{
  std::vector<glm::vec3> positions;
  positions.reserve(12);
  
  const glm::vec3 p1(60.f, 60.f, 0.f);
  const glm::vec3 p2(60.f, 100.f, 0.f);
  const glm::vec3 p3(100.f, 100.f, 0.f);
  const glm::vec3 p4(100.f, 60.f, 0.f);

  positions.push_back(p1);
  positions.push_back(p2);
  positions.push_back(p3);

  positions.push_back(p1);
  positions.push_back(p3);
  positions.push_back(p4);

  positions.push_back(p1);
  positions.push_back(p3);
  positions.push_back(p2);

  positions.push_back(p1);
  positions.push_back(p4);
  positions.push_back(p3);

  setGeometry(positions);

  setTechnique( clPipeline::colorFrameType,
                std::make_unique<clPipeline::UnlightedColorTechnique>(
                                          false,
                                          false,
                                          VK_COMPARE_OP_ALWAYS,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST));
}

std::optional<glm::vec3> PlaneManipulator::tryActivate(
                                            const TouchInfo& touchInfo) noexcept
{
  std::optional<glm::vec3> intersecPoint =
                                PlaneMove3DManipulator::tryActivate(touchInfo);

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
      Abort("PlaneManipulator::tryActivate: unable to lock undo group");
    }
  }

  return intersecPoint;
}

void PlaneManipulator::processShift(const glm::vec3& startTouchPoint,
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
    Log() << "PlaneManipulator::processShift: " << error.what();
  }
  catch(...)
  {
    Log() << "PlaneManipulator::processShift: unknown error";
  }
}

void PlaneManipulator::release() noexcept
{
  _groupCommandLocker.reset();
  PlaneMove3DManipulator::release();
}
