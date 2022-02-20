#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/render/Drawable/AntiscaleDrawableModificator.h>

using namespace mtt;

Object3DRenderObserver::Object3DRenderObserver( Object3D& object,
                                                CommonEditData& commonData) :
  AbstractObjectRenderObserver(object, commonData)
{
  connect(&object,
          &Object3D::transformChanged,
          this,
          &Object3DRenderObserver::_setObjectTransform,
          Qt::DirectConnection);
  _setObjectTransform(object.localToWorldTransform());
}

void Object3DRenderObserver::_setObjectTransform(
                                            const glm::mat4& transform) noexcept
{
  glm::mat4 positionTransform(1);
  positionTransform[3] = transform[3];
  _positionJoint.setJointMatrix(positionTransform);

  _positionRotateJoint.setJointMatrix(
                    AntiscaleDrawableModificator::getCutedTransform(transform));

  _fullTransformJoint.setJointMatrix(transform);
}
