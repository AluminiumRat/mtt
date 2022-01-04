#include <glm/gtx/transform.hpp>

#include <Manipulator/MoveManipulator.h>
#include <Objects/MovableObject.h>

MoveManipulator::MoveManipulator( MovableObject& object,
                                  mtt::UndoStack& undoStack) :
  ObjectManipulator(object),
  _xAxisManipulator(object, undoStack),
  _xPlaneManipulator(object, undoStack),
  _yAxisManipulator(object, undoStack),
  _yPlaneManipulator(object, undoStack),
  _zAxisManipulator(object, undoStack),
  _zPlaneManipulator(object, undoStack)
{
  glm::mat4 xManipulatorTransform = glm::rotate(glm::radians(90.f),
                                                glm::vec3(0, 0, 1));
  xManipulatorTransform =
                glm::rotate(glm::radians(90.f), glm::vec3(0, 1, 0)) *
                                                          xManipulatorTransform;
  _xManipulatorJoint.setJointMatrix(xManipulatorTransform);
  _xAxisManipulator.setUnselectedColor(xUnselectedColor);
  _xAxisManipulator.setHighlihtedColor(xHighlihtedColor);
  _xAxisManipulator.setActivatedColor(xActivatedColor);
  _xManipulatorJoint.addChild(_xAxisManipulator.node());
  registerSubmanipulator(_xAxisManipulator, _xManipulatorJoint);

  _xPlaneManipulator.setUnselectedColor(xUnselectedColor);
  _xPlaneManipulator.setHighlihtedColor(xHighlihtedColor);
  _xPlaneManipulator.setActivatedColor(xActivatedColor);
  _xManipulatorJoint.addChild(_xPlaneManipulator.node());
  registerSubmanipulator(_xPlaneManipulator, _xManipulatorJoint);

  glm::mat4 yManipulatorTransform = glm::rotate(glm::radians(-90.f),
                                                glm::vec3(0, 0, 1));
  yManipulatorTransform =
              glm::rotate(glm::radians(-90.f), glm::vec3(1, 0, 0)) *
                                                          yManipulatorTransform;
  _yManipulatorJoint.setJointMatrix(yManipulatorTransform);
  _yAxisManipulator.setUnselectedColor(yUnselectedColor);
  _yAxisManipulator.setHighlihtedColor(yHighlihtedColor);
  _yAxisManipulator.setActivatedColor(yActivatedColor);
  _yManipulatorJoint.addChild(_yAxisManipulator.node());
  registerSubmanipulator(_yAxisManipulator, _yManipulatorJoint);

  _yPlaneManipulator.setUnselectedColor(yUnselectedColor);
  _yPlaneManipulator.setHighlihtedColor(yHighlihtedColor);
  _yPlaneManipulator.setActivatedColor(yActivatedColor);
  _yManipulatorJoint.addChild(_yPlaneManipulator.node());
  registerSubmanipulator(_yPlaneManipulator, _yManipulatorJoint);

  _zAxisManipulator.setUnselectedColor(zUnselectedColor);
  _zAxisManipulator.setHighlihtedColor(zHighlihtedColor);
  _zAxisManipulator.setActivatedColor(zActivatedColor);
  registerSubmanipulator(_zAxisManipulator, _zAxisManipulator.node());

  _zPlaneManipulator.setUnselectedColor(zUnselectedColor);
  _zPlaneManipulator.setHighlihtedColor(zHighlihtedColor);
  _zPlaneManipulator.setActivatedColor(zActivatedColor);
  registerSubmanipulator(_zPlaneManipulator, _zPlaneManipulator.node());
}
