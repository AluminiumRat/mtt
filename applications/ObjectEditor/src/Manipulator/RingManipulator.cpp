#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/clPipeline/MeshTechniques/UnlightedColorTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/editorLib/Objects/RotatableObject.h>
#include <mtt/render/Drawable/AntiscaleDrawableModificator.h>
#include <mtt/utilities/Abort.h>

#include <Manipulator/RingManipulator.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>

#define SECTORS_NUMBER 64
#define SIDES_NUMBER 12
#define RING_RADIUS 80.f
#define LINE_RADIUS 5.f

static void addSide(std::vector<glm::vec3>& positions,
                    float sectorStartAngle,
                    float sectorEndAngle,
                    float sideStartAngle,
                    float sideEndAngle)
{
  glm::vec4 sideStart = glm::vec4(cos(sideStartAngle) * LINE_RADIUS,
                                  0.f,
                                  sin(sideStartAngle) * LINE_RADIUS,
                                  1);
  sideStart += glm::vec4(RING_RADIUS, 0, 0, 0);
                                                                    ;
  glm::vec4 sideEnd = glm::vec4(cos(sideEndAngle) * LINE_RADIUS,
                                0.f,
                                sin(sideEndAngle) * LINE_RADIUS,
                                1);
  sideEnd += glm::vec4(RING_RADIUS, 0, 0, 0);

  glm::mat4x4 sectorStartRotation = glm::rotate(sectorStartAngle,
                                                glm::vec3(0, 0, 1));
  glm::mat4x4 sectorEndRotation = glm::rotate(sectorEndAngle,
                                              glm::vec3(0, 0, 1));

  glm::vec3 p1 = sectorStartRotation * sideStart;
  glm::vec3 p2 = sectorStartRotation * sideEnd;
  glm::vec3 p3 = sectorEndRotation * sideStart;
  glm::vec3 p4 = sectorEndRotation * sideEnd;

  positions.push_back(p1);
  positions.push_back(p2);
  positions.push_back(p3);
  positions.push_back(p3);
  positions.push_back(p2);
  positions.push_back(p4);
}

static void addSector(std::vector<glm::vec3>& positions,
                      float startAngle,
                      float endAngle)
{
  float angleStep = 2.f * glm::pi<float>() / SIDES_NUMBER;
  float currentAngle = 0;
  for (size_t index = 1; index < SIDES_NUMBER; index++)
  {
    float nextAngle = currentAngle + angleStep;
    addSide(positions, startAngle, endAngle, currentAngle, nextAngle);
    currentAngle = nextAngle;
  }
  addSide(positions, startAngle, endAngle, currentAngle, 0);
}

RingManipulator::RingManipulator( mtt::RotatableObject& object,
                                  mtt::UndoStack& undoStack) :
  RingRotation3DManipulator(
                          mtt::AutoscaleDrawableModificator::PIXEL_SCALE_MODE),
  _undoStack(undoStack),
  _object(object)
{
  std::vector<glm::vec3> positions;
  positions.reserve(SECTORS_NUMBER * SIDES_NUMBER * 6);
  
  float angleStep = 2.f * glm::pi<float>() / SECTORS_NUMBER;
  float currentAngle = 0;
  for(size_t index = 1; index < SECTORS_NUMBER; index++)
  {
    float nextAngle = currentAngle + angleStep;
    addSector(positions, currentAngle, nextAngle);
    currentAngle = nextAngle;
  }
  addSector(positions, currentAngle, 0);

  setGeometry(positions);
  setTechnique( mtt::clPipeline::colorFrameType,
                std::make_unique<mtt::clPipeline::UnlightedColorTechnique>(
                                          false,
                                          false,
                                          VK_COMPARE_OP_ALWAYS,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST));
}

std::optional<glm::vec3> RingManipulator::tryActivate(
                                            const TouchInfo& touchInfo) noexcept
{
  std::optional<glm::vec3> intersecPoint =
                              RingRotation3DManipulator::tryActivate(touchInfo);

  if(intersecPoint.has_value())
  {
    _startObjectToWorld = _object.coordSystemTransform();
    _startWorldToObject = glm::inverse(_object.coordSystemTransform());
    _startRotation = glm::mat4_cast(_object.rotation());
    try
    {
      _groupCommandLocker.reset(_undoStack);
    }
    catch(...)
    {
      mtt::Abort("RingManipulator::tryActivate: unable to lock undo group");
    }
  }

  return intersecPoint;
}

void RingManipulator::processRotation(const glm::mat4& rotation) noexcept
{
  glm::mat4 rotationInLocal =
                          _startWorldToObject * rotation * _startObjectToWorld;
  rotationInLocal =
          mtt::AntiscaleDrawableModificator::getCutedTransform(rotationInLocal);

  glm::quat newRotation(rotationInLocal * _startRotation);
  if(_object.rotation() == newRotation) return;

  try
  {
    using Command = mtt::SetPropertyCommand<
                              mtt::RotatableObject,
                              glm::quat,
                              void (mtt::RotatableObject::*)(const glm::quat&)>;
    std::unique_ptr<Command> command(
                                new Command(_object,
                                            &mtt::RotatableObject::setRotation,
                                            _object.rotation(),
                                            newRotation));
    _undoStack.addAndMake(std::move(command));
  }
  catch(std::exception& error)
  {
    mtt::Log() << "RingManipulator::processRotation: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "RingManipulator::processRotation: unknown error";
  }
}

void RingManipulator::release() noexcept
{
  _groupCommandLocker.reset();
  RingRotation3DManipulator::release();
}
