#pragma once

#include <functional>
#include <map>
#include <memory>

#include <mtt/application/Scene/Object.h>
#include <mtt/utilities/Abort.h>

namespace mtt
{
  class ObjectFactory
  {
  public:
    using TypeIndex = uint16_t;

    static constexpr TypeIndex notIndex = 0;

  public:
    ObjectFactory() = default;
    ObjectFactory(const ObjectFactory&) = delete;
    ObjectFactory& operator = (const ObjectFactory&) = delete;
    virtual ~ObjectFactory() noexcept = default;

    template<typename ObjectClass, uint16_t typeIndex>
    inline void registerType();

    /// Returns typeIndex of objects class or nearest registered parent
    /// Returns notIndex if the object class and all parents are not registered
    TypeIndex getTypeIndex(const Object& object) const noexcept;

    /// Returns nullptr if typeIndex is not registered
    std::unique_ptr<Object> createObject( TypeIndex typeIndex,
                                          const QString& name,
                                          bool canBeRenamed,
                                          const UID& uid) const;

  private:
    using Builder = std::function<std::unique_ptr<Object>(const QString&,
                                                          bool,
                                                          const UID&)>;
    using Builders = std::map<TypeIndex, Builder>;
    Builders _builders;

    using TypeIndices = std::map<QString, TypeIndex>;
    TypeIndices _typeIndices;
  };

  template<typename ObjectClass, uint16_t typeIndex>
  inline void ObjectFactory::registerType()
  {
    if(_builders.find(typeIndex) != _builders.end()) Abort("ObjectFactory::registerType: typeIndex is already registered.");
    _builders[typeIndex] =  []( const QString& name,
                                bool canBeRenamed,
                                const UID& uid) -> std::unique_ptr<Object>
                            {
                              return std::make_unique<ObjectClass>(
                                                                  name,
                                                                  canBeRenamed,
                                                                  uid);
                            };

    QString className = ObjectClass::staticMetaObject.className();
    if(_typeIndices.find(className) != _typeIndices.end()) Abort("ObjectFactory::registerType: class is already registered.");
    _typeIndices[className] = typeIndex;
  }
}