#include <glm/gtx/transform.hpp>

#include <mtt/editorLib/Objects/ScalableObject.h>

#include <Manipulator/ScaleManipulator.h>

ScaleManipulator::ScaleManipulator( mtt::ScalableObject& object,
                                    mtt::UndoStack& undoStack) :
  ObjectManipulator(object),
  _xPlaneManipulator(object, undoStack),
  _yPlaneManipulator(object, undoStack),
  _zPlaneManipulator(object, undoStack)
{
  glm::mat4 xManipulatorTransform = glm::rotate(glm::radians(90.f),
                                                glm::vec3(0, 0, 1));
  xManipulatorTransform =
                glm::rotate(glm::radians(90.f), glm::vec3(0, 1, 0)) *
                                                          xManipulatorTransform;
  _xManipulatorJoint.setJointMatrix(xManipulatorTransform);

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

  _yPlaneManipulator.setUnselectedColor(yUnselectedColor);
  _yPlaneManipulator.setHighlihtedColor(yHighlihtedColor);
  _yPlaneManipulator.setActivatedColor(yActivatedColor);
  _yManipulatorJoint.addChild(_yPlaneManipulator.node());
  registerSubmanipulator(_yPlaneManipulator, _yManipulatorJoint);

  _zPlaneManipulator.setUnselectedColor(zUnselectedColor);
  _zPlaneManipulator.setHighlihtedColor(zHighlihtedColor);
  _zPlaneManipulator.setActivatedColor(zActivatedColor);
  registerSubmanipulator(_zPlaneManipulator, _zPlaneManipulator.node());
}
