#pragma once

#include <stdexcept>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/animation/ValueKeypoint.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/application/Scene/ObjectFactory.h>
#include <mtt/application/Scene/ObjectVisitor.h>
#include <mtt/application/DataStream.h>
#include <mtt/application/TimeT.h>
#include <mtt/utilities/static_cast_unique_ptr.h>

namespace mtt
{
  class ObjectLoader : public ObjectVisitor
  {
  public:
    ObjectLoader();
    ObjectLoader(const ObjectLoader&) = delete;
    ObjectLoader& operator = (const ObjectLoader&) = delete;
    virtual ~ObjectLoader() noexcept = default;

    template<typename ObjectType>
    inline std::unique_ptr<ObjectType> loadObject(
                                            bool canBeRenamed,
                                            mtt::DataStream& stream,
                                            const QDir& fileDirectory,
                                            UID::ValueType mixUIDValue,
                                            const ObjectFactory& objectFactory);
    /// Load object data without type index and id
    void loadEmbeddedObject(Object& object,
                            DataStream& stream,
                            const QDir& fileDirectory,
                            UID::ValueType mixUIDValue,
                            const ObjectFactory& objectFactory);

    inline DataStream& stream() const noexcept;
    inline const QDir& fileDirectory() const noexcept;
    inline const UID::ValueType& mixUIDValue() const noexcept;
    inline const ObjectFactory& objectFactory() const noexcept;

    virtual void visitObjectGroup(ObjectGroup& object) override;

  protected:
    QString readFilename();
    template<typename ValueType>
    inline void readKeypoint(ValueKeypoint<ValueType, TimeT>& keypoint);
    UID readUID();
    template<typename GroupClass, typename ChildClass>
    inline void readChilds(GroupClass& parent, bool canBeRenamed);

  private:
    void _readObjectData( Object& object,
                          DataStream& stream,
                          const QDir& fileDirectory,
                          UID::ValueType mixUIDValue,
                          const ObjectFactory& objectFactory);

  private:
    DataStream* _stream;
    QDir _fileDirectory;
    UID::ValueType _mixUIDValue;
    const ObjectFactory* _objectFactory;
  };

  template<typename ObjectType>
  inline std::unique_ptr<ObjectType> ObjectLoader::loadObject(
                                            bool canBeRenamed,
                                            DataStream& stream,
                                            const QDir& fileDirectory,
                                            UID::ValueType mixUIDValue,
                                            const ObjectFactory& objectFactory)
  {
    uint16_t typeIndex;
    stream >> typeIndex;

    UID id = stream.readUID().mixedUID(mixUIDValue);

    QString name;
    stream >> name;

    std::unique_ptr<Object> object = objectFactory.createObject(typeIndex,
                                                                name,
                                                                canBeRenamed,
                                                                id);
    if (dynamic_cast<ObjectType*>(object.get()) == nullptr)
    {
      std::string errorString("ObjectLoader: Object type index ");
      errorString += std::to_string(typeIndex);
      errorString += " does not match requested class ";
      errorString += ObjectType::staticMetaObject.className();
      throw std::runtime_error(errorString);
    }

    _readObjectData(*object, stream, fileDirectory, mixUIDValue, objectFactory);

    return static_cast_unique_ptr<ObjectType>(std::move(object));
  }

  inline DataStream& ObjectLoader::stream() const noexcept
  {
    return *_stream;
  }

  inline const QDir& ObjectLoader::fileDirectory() const noexcept
  {
    return _fileDirectory;
  }

  inline const UID::ValueType& ObjectLoader::mixUIDValue() const noexcept
  {
    return _mixUIDValue;
  }

  inline const ObjectFactory& ObjectLoader::objectFactory() const noexcept
  {
    return *_objectFactory;
  }

  template<typename ValueType>
  inline void ObjectLoader::readKeypoint(
                                      ValueKeypoint<ValueType, TimeT>& keypoint)
  {
    TimeT time = _stream->readTime();
    keypoint.setTime(time);

    ValueType value;
    *_stream >> value;
    keypoint.setValue(value);

    uint8_t interpolation = _stream->readInt8();
    keypoint.setInterpolation(InterpolationType(interpolation));
  }

  template<typename GroupClass, typename ChildClass>
  inline void ObjectLoader::readChilds(GroupClass& parent, bool canBeRenamed)
  {
    uint32_t objectsNumber = _stream->readUint32();
    for (; objectsNumber != 0; objectsNumber--)
    {
      parent.addChild(loadObject<ChildClass>( canBeRenamed,
                                              stream(),
                                              fileDirectory(),
                                              mixUIDValue(),
                                              objectFactory()));
    }
  }
}