#pragma once

#include <stdexcept>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/application/DataStream.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/editorLib/Objects/CEVisitor.h>
#include <mtt/editorLib/Objects/ObjectFactory.h>
#include <mtt/utilities/static_cast_unique_ptr.h>

namespace mtt
{
  class ObjectLoader : public CEVisitor
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

    inline DataStream& stream() const noexcept;
    inline const QDir& fileDirectory() const noexcept;
    inline const UID::ValueType& mixUIDValue() const noexcept;
    inline const ObjectFactory& objectFactory() const noexcept;

    virtual void visitAmbientLightObject(AmbientLightObject& object) override;
    virtual void visitAnimationObject(AnimationObject& object) override;
    virtual void visitAnimationTrack(AnimationTrack& object) override;
    virtual void visitBackgroundObject(BackgroundObject& object) override;
    virtual void visitCubemapObject(CubemapObject& object) override;
    virtual void visitDirectLightObject(DirectLightObject& object) override;
    virtual void visitDisplayedObject(DisplayedObject& object) override;
    virtual void visitEnvironmentModel(EnvironmentModel& object) override;
    virtual void visitLightObject(LightObject& object) override;
    virtual void visitMovableObject(MovableObject& object) override;
    virtual void visitRotatableObject(RotatableObject& object) override;
    virtual void visitScalableObject(ScalableObject& object) override;
    virtual void visitSkeletonObject(SkeletonObject& object) override;

  protected:
    QString readFilename();
    template<typename ValueType>
    inline void readKeypoint( ValueKeypoint<ValueType,
                              AnimationTrack::TimeType>& keypoint);
    void readCubemapData(CubemapObject& object);
    UID readUID();

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
      throw std::runtime_error("Object type index does not match requested class");
    }

    DataStream* oldStream = _stream;
    _stream = &stream;

    QDir oldFileDirectory = _fileDirectory;
    _fileDirectory = fileDirectory;

    UID::ValueType oldMixUIDValue = _mixUIDValue;
    _mixUIDValue = mixUIDValue;

    const ObjectFactory* oldFactory = _objectFactory;
    _objectFactory = &objectFactory;

    process(*object);

    _stream = oldStream;
    _fileDirectory = oldFileDirectory;
    _mixUIDValue = oldMixUIDValue;
    _objectFactory = oldFactory;

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
  inline void ObjectLoader::readKeypoint( ValueKeypoint<ValueType,
                                          AnimationTrack::TimeType>& keypoint)
  {
    uint32_t timeCount = _stream->readUint32();
    AnimationTrack::TimeType time(timeCount);
    keypoint.setTime(time);

    ValueType value;
    *_stream >> value;
    keypoint.setValue(value);

    uint8_t interpolation = _stream->readInt8();
    keypoint.setInterpolation(InterpolationType(interpolation));
  }
}