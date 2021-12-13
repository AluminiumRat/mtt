#pragma once

#include <cstdint>
#include <memory>
#include <stdexcept>

#include <mtt/Application/Scene/Object.h>

class ObjectBuilder
{
public:
  enum class ObjectType : uint16_t
  {
    Object = 0,
    Material = 1,
    Skeleton = 2,
    LOD = 3,
    Mesh = 4,
    Animation = 5,
    AnimationTrack = 6
  };

  static ObjectType getObjectType(const mtt::Object& object) noexcept;

  template<typename ObjectClass>
  inline static std::unique_ptr<ObjectClass> buildObject( ObjectType type,
                                                          QString name,
                                                          bool canBeRenamed,
                                                          mtt::UID id);

private:
  struct ObjectTypeParser;

  static std::unique_ptr<mtt::Object> _buildObject( ObjectType type,
                                                    QString name,
                                                    bool canBeRenamed,
                                                    mtt::UID id);
};

template<typename ObjectClass>
inline std::unique_ptr<ObjectClass> ObjectBuilder::buildObject(
                                                              ObjectType type,
                                                              QString name,
                                                              bool canBeRenamed,
                                                              mtt::UID id)
{
  std::unique_ptr<mtt::Object> rawObject = _buildObject(type,
                                                        name,
                                                        canBeRenamed,
                                                        id);
  ObjectClass* objectPtr = dynamic_cast<ObjectClass*>(rawObject.get());
  if (objectPtr == nullptr)
  {
    throw std::runtime_error("Object type id does not match requested class");
  }

  return std::unique_ptr<ObjectClass>(
                                static_cast<ObjectClass*>(rawObject.release()));
}
