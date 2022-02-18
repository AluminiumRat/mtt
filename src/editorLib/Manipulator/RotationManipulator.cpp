#include <glm/gtx/transform.hpp>

#include <mtt/editorLib/Manipulator/RotationManipulator.h>
#include <mtt/editorLib/Objects/RotatableObject.h>

using namespace mtt;

RotationManipulator::RotationManipulator( RotatableObject& object,
                                          UndoStack& undoStack) :
  ObjectManipulator(object),
  _xManipulator(object, undoStack),
  _yManipulator(object, undoStack),
  _zManipulator(object, undoStack)
{
  glm::mat4 xManipulatorTransform = glm::rotate(glm::radians(90.f),
                                                glm::vec3(0, 1, 0));
  _xManipulatorJoint.setJointMatrix(xManipulatorTransform);
  _xManipulator.setUnselectedColor(xUnselectedColor);
  _xManipulator.setHighlihtedColor(xHighlihtedColor);
  _xManipulator.setActivatedColor(xActivatedColor);
  _xManipulatorJoint.addChild(_xManipulator.node());
  registerSubmanipulator(_xManipulator, _xManipulatorJoint);

  glm::mat4 yManipulatorTransform = glm::rotate(glm::radians(-90.f),
                                                glm::vec3(1, 0, 0));
  _yManipulatorJoint.setJointMatrix(yManipulatorTransform);
  _yManipulator.setUnselectedColor(yUnselectedColor);
  _yManipulator.setHighlihtedColor(yHighlihtedColor);
  _yManipulator.setActivatedColor(yActivatedColor);
  _yManipulatorJoint.addChild(_yManipulator.node());
  registerSubmanipulator(_yManipulator, _yManipulatorJoint);

  _zManipulator.setUnselectedColor(zUnselectedColor);
  _zManipulator.setHighlihtedColor(zHighlihtedColor);
  _zManipulator.setActivatedColor(zActivatedColor);
  registerSubmanipulator(_zManipulator, _zManipulator.node());
}
