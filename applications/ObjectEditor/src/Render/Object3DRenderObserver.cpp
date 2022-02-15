#include <mtt/render/Drawable/AntiscaleDrawableModificator.h>
#include <Render/Object3DRenderObserver.h>

Object3DRenderObserver::Object3DRenderObserver( mtt::Object3D& object,
                                                EditorCommonData& commonData) :
  AbstractObjectRenderObserver(object, commonData)
{
  connect(&object,
          &mtt::Object3D::transformChanged,
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
              mtt::AntiscaleDrawableModificator::getCutedTransform(transform));

  _fullTransformJoint.setJointMatrix(transform);
}
