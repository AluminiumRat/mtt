#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/dlPipeline/MeshTechniques/InstrumentalTechnique.h>
#include <mtt/dlPipeline/constants.h>
#include <mtt/utilities/Abort.h>

#include <Objects/ScalableObject.h>
#include <Manipulator/TriangleManipulator.h>

TriangleManipulator::TriangleManipulator( ScalableObject& object,
                                          mtt::UndoStack& undoStack) :
  UniformScale3DManipulator(
                          mtt::AutoscaleDrawableModificator::PIXEL_SCALE_MODE),
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
  setTechnique( mtt::dlPipeline::colorFrameType,
                std::make_unique<mtt::dlPipeline::InstrumentalTechnique>(
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
      mtt::Abort("TriangleManipulator::tryActivate: unable to lock undo group");
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
    using Command = mtt::SetPropertyCommand<
                                    ScalableObject,
                                    glm::vec3,
                                    void (ScalableObject::*)(const glm::vec3&)>;
    std::unique_ptr<Command> command(new Command( _object,
                                                  &ScalableObject::setScale,
                                                  _object.scale(),
                                                  newScale));
    _undoStack.addAndMake(std::move(command));
  }
  catch(std::exception& error)
  {
    mtt::Log() << "TriangleManipulator::processScale: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "TriangleManipulator::processScale: unknown error";
  }
}

void TriangleManipulator::release() noexcept
{
  _groupCommandLocker.reset();
  UniformScale3DManipulator::release();
}
