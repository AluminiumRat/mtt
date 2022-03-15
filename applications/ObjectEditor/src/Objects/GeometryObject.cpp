#include <mtt/utilities/Abort.h>

#include <Objects/GeometryObject.h>

GeometryObject::GeometryObject( const QString& name,
                                bool canBeRenamed,
                                const mtt::UID& id) :
  ScalableObject(name, canBeRenamed, id),
  _skeletonLink(*this)
{
}

void GeometryObject::_connectSkeleton(mtt::SkeletonObject& skeleton)
{
  connect(&skeleton,
          &mtt::SkeletonObject::transformChanged,
          this,
          &GeometryObject::setCoordSystemTransform,
          Qt::DirectConnection);
  setCoordSystemTransform(skeleton.localToWorldTransform());
}

void GeometryObject::_disconnectSkeleton(mtt::SkeletonObject& skeleton) noexcept
{
  disconnect( &skeleton,
              &mtt::SkeletonObject::transformChanged,
              this,
              &GeometryObject::setCoordSystemTransform);
  setCoordSystemTransform(glm::mat4(1));
}
