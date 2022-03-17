#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/clPipeline/MeshTechniques/UnlightedColorTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/editorLib/Manipulator/TriangleManipulator.h>
#include <mtt/editorLib/Objects/ScalableObject.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

TriangleManipulator::TriangleManipulator( ScalableObject& object,
                                          UndoStack& undoStack) :
  UniformScale3DManipulator(AutoscaleDrawableModificator::PIXEL_SCALE_MODE),
  _undoStack(undoStack),
  _object(object),
  _startScale(1.f)
{
  std::vector<glm::vec3> positions;
  positions.reserve(6);

  const glm::vec3 p1(0.f, 0.f, 0.f);
  const glm::vec3 p2(100.f, 0.f, 0.f);
  const glm::vec3 p3(0.f, 100.f, 0.f);

  positions.push_back(p1);
  positions.push_back(p2);
  positions.push_back(p3);

  positions.push_back(p1);
  positions.push_back(p3);
  positions.push_back(p2);

  setGeometry(positions);
  setTechnique( clPipeline::colorFrameType,
                std::make_unique<clPipeline::UnlightedColorTechnique>(
                                          false,
                                          false,
                                          VK_COMPARE_OP_ALWAYS,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST));
}

std::optional<glm::vec3> TriangleManipulator::tryActivate(
                                            const TouchInfo& touchInfo) noexcept
{
  std::optional<glm::vec3> intersecPoint =
                              UniformScale3DManipulator::tryActivate(touchInfo);

  if(intersecPoint.has_value())
  {
    _startScale = _object.scale();
    try
    {
      _groupCommandLocker.reset(_undoStack);
    }
    catch(...)
    {
      Abort("TriangleManipulator::tryActivate: unable to lock undo group");
    }
  }

  return intersecPoint;
}

void TriangleManipulator::processScale(float scale) noexcept
{
  glm::vec3 newScale = _startScale * scale;
  if(_object.scale() == newScale) return;

  try
  {
    std::unique_ptr<AbstractEditCommand> command =
                              makeSetPropertyCommand( _object,
                                                      &ScalableObject::scale,
                                                      &ScalableObject::setScale,
                                                      newScale);
    _undoStack.addAndMake(std::move(command));
  }
  catch(std::exception& error)
  {
    Log() << "TriangleManipulator::processScale: " << error.what();
  }
  catch(...)
  {
    Log() << "TriangleManipulator::processScale: unknown error";
  }
}

void TriangleManipulator::release() noexcept
{
  _groupCommandLocker.reset();
  UniformScale3DManipulator::release();
}
