#pragma once

#include <optional>

#include <mtt/Core/Scene/ObjectLink.h>

#include <Objects/ScalableObject.h>
#include <Objects/SkeletonObject.h>

class GeometryObject : public ScalableObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  explicit GeometryObject(const mtt::UID& id = mtt::UID());
  GeometryObject(const GeometryObject&) = delete;
  GeometryObject& operator = (const GeometryObject&) = delete;
  virtual ~GeometryObject() noexcept = default;

  inline const mtt::ObjectRef<SkeletonObject>& skeletonRef() const noexcept;
  inline SkeletonObject* skeleton() const noexcept;
  /// You can use nullptr to remove link
  void setSkeleton(SkeletonObject* skeleton);
  /// You can use invalid UID to remove link
  void setSkeletonId(const mtt::UID& id);

signals:
  void skeletonRefChanged(SkeletonObject* skeleton);

private:
  void _connectSkeleton(SkeletonObject& skeleton);
  void _disconnectSkeleton(SkeletonObject& skeleton) noexcept;

private:
  using SkeletonLink = mtt::ObjectLink< SkeletonObject,
                                        GeometryObject,
                                        &GeometryObject::_connectSkeleton,
                                        &GeometryObject::_disconnectSkeleton>;
  std::optional<SkeletonLink> _skeletonLink;
};

inline const mtt::ObjectRef<SkeletonObject>&
                                  GeometryObject::skeletonRef() const noexcept
{
  return *_skeletonLink;
}

inline SkeletonObject* GeometryObject::skeleton() const noexcept
{
  return _skeletonLink->get();
}
