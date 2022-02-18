#include <mtt/render/Drawable/AntiscaleDrawableModificator.h>
#include <mtt/editorLib/Manipulator/ObjectManipulator.h>
#include <mtt/editorLib/Objects/Object3D.h>

using namespace mtt;

ObjectManipulator::ObjectManipulator(Object3D& object) :
  _object(object),
  _orientation(LOCAL_ORIENTATION)
{
  connect(&_object,
          &Object3D::transformChanged,
          this,
          &ObjectManipulator::_updatePosition,
          Qt::DirectConnection);
  _updatePosition();
}

void ObjectManipulator::_updatePosition() noexcept
{
  if(_orientation == LOCAL_ORIENTATION)
  {
    glm::mat4 newTransform =
            AntiscaleDrawableModificator::getCutedTransform(
                                              _object.localToWorldTransform());
    node().setTransformMatrix(newTransform);
  }
  else
  {
    glm::mat4 newTransform(1);
    newTransform[3] = _object.localToWorldTransform()[3];
    node().setTransformMatrix(newTransform);
  }
}

void ObjectManipulator::setOrientation(Orientation newOrientation) noexcept
{
  if(_orientation == newOrientation) return;
  _orientation = newOrientation;
  _updatePosition();
  emit orientationChanged(_orientation);
}
