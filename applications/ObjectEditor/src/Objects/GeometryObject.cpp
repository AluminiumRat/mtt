#include <mtt/Utilities/Abort.h>

#include <Objects/GeometryObject.h>

GeometryObject::GeometryObject( const QString& name,
                                bool canBeRenamed,
                                const mtt::UID& id) :
  ScalableObject(name, canBeRenamed, id),
  _skeletonLink(*this)
{
}

void GeometryObject::setSkeleton(SkeletonObject* skeleton)
{
  if(skeleton == nullptr) setSkeletonId(mtt::UID());
  else setSkeletonId(skeleton->id());
}

void GeometryObject::setSkeletonId(const mtt::UID& id)
{
  try
  {
    _skeletonLink.setReferencedId(id);
  }
  catch(...)
  {
    try
    {
      _skeletonLink.setReferencedId(mtt::UID());
    }
    catch(...)
    {
      mtt::Abort("GeometryObject::setSkeletonId: unable to emplace skeleton link");
    }
    throw;
  }
}

void GeometryObject::_connectSkeleton(SkeletonObject& skeleton)
{
  connect(&skeleton,
          &SkeletonObject::transformChanged,
          this,
          &GeometryObject::setCoordSystemTransform,
          Qt::DirectConnection);
  setCoordSystemTransform(skeleton.localToWorldTransform());
  emit skeletonRefChanged(&skeleton);
}

void GeometryObject::_disconnectSkeleton(SkeletonObject& skeleton) noexcept
{
  disconnect( &skeleton,
              &SkeletonObject::transformChanged,
              this,
              &GeometryObject::setCoordSystemTransform);
  setCoordSystemTransform(glm::mat4(1));
  emit skeletonRefChanged(nullptr);
}
