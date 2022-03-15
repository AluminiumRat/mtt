#pragma once

#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/editorLib/Objects/ScalableObject.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>

#include <Objects/OEVisitorExtension.h>

class GeometryObject : public mtt::ScalableObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension,
                          visitGeometryObject,
                          visitConstGeometryObject,
                          mtt::ScalableObject)

public:
  GeometryObject( const QString& name,
                  bool canBeRenamed,
                  const mtt::UID& id = mtt::UID());
  GeometryObject(const GeometryObject&) = delete;
  GeometryObject& operator = (const GeometryObject&) = delete;
  virtual ~GeometryObject() noexcept = default;

  inline mtt::ObjectRef<mtt::SkeletonObject>& skeletonRef() noexcept;
  inline const mtt::ObjectRef<mtt::SkeletonObject>&
                                                  skeletonRef() const noexcept;

signals:
  void skeletonRefChanged(mtt::SkeletonObject* skeleton);

private:
  void _connectSkeleton(mtt::SkeletonObject& skeleton);
  void _disconnectSkeleton(mtt::SkeletonObject& skeleton) noexcept;

private:
  using SkeletonLink = mtt::ObjectLink< mtt::SkeletonObject,
                                        GeometryObject,
                                        &GeometryObject::_connectSkeleton,
                                        &GeometryObject::_disconnectSkeleton,
                                        &GeometryObject::skeletonRefChanged>;
  SkeletonLink _skeletonLink;
};

inline mtt::ObjectRef<mtt::SkeletonObject>&
                                          GeometryObject::skeletonRef() noexcept
{
  return _skeletonLink;
}

inline const mtt::ObjectRef<mtt::SkeletonObject>&
                                  GeometryObject::skeletonRef() const noexcept
{
  return _skeletonLink;
}
