# pragma once

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/animation/ValueKeypoint.h>
#include <mtt/application/Scene/ObjectFactory.h>
#include <mtt/application/Scene/ObjectVisitor.h>
#include <mtt/application/DataStream.h>
#include <mtt/application/TimeT.h>

namespace mtt
{
  class ObjectSaver : public ObjectVisitor
  {
  public:
    enum FilenameWriteMode
    {
      WRITE_RELATIVE_FILE_PATH,
      WRITE_FULL_FILE_PATH
    };

  public:
    ObjectSaver();
    ObjectSaver(const ObjectSaver&) = delete;
    ObjectSaver& operator = (const ObjectSaver&) = delete;
    virtual ~ObjectSaver() noexcept = default;

    void saveObject(const Object& object,
                    DataStream& stream,
                    const QDir& fileDirectory,
                    const ObjectFactory& objectFactory);
    /// Save without type index and id.
    /// For embedded objects.
    void saveObjectData(const Object& object,
                        DataStream& stream,
                        const QDir& fileDirectory,
                        const ObjectFactory& objectFactory);

    inline FilenameWriteMode filenameWriteMode() const noexcept;
    inline void setFilenameWriteMode(FilenameWriteMode newValue) noexcept;

    virtual void visitConstObject(const Object& object) override;
    virtual void visitConstObjectGroup(const ObjectGroup& object) override;

  protected:
    inline DataStream& stream() const noexcept;
    inline const QDir& fileDirectory() const noexcept;
    inline const ObjectFactory& objectFactory() const noexcept;

    void writeFilename(const QString& filename);
    template<typename ValueType>
    inline void writeKeypoint(ValueKeypoint<ValueType, TimeT> keypoint);

    void writeChilds(const Object& parent);

  private:
    DataStream* _stream;
    QDir _fileDirectory;
    const ObjectFactory* _objectFactory;
    FilenameWriteMode _filenameWriteMode;
  };

  inline ObjectSaver::FilenameWriteMode
                                ObjectSaver::filenameWriteMode() const noexcept
  {
    return _filenameWriteMode;
  }

  inline void ObjectSaver::setFilenameWriteMode(
                                            FilenameWriteMode newValue) noexcept
  {
    _filenameWriteMode = newValue;
  }

  inline DataStream& ObjectSaver::stream() const noexcept
  {
    return *_stream;
  }

  inline const QDir& ObjectSaver::fileDirectory() const noexcept
  {
    return _fileDirectory;
  }

  inline const ObjectFactory& ObjectSaver::objectFactory() const noexcept
  {
    return *_objectFactory;
  }

  template<typename ValueType>
  inline void ObjectSaver::writeKeypoint(
                                      ValueKeypoint<ValueType, TimeT> keypoint)
  {
    *_stream << keypoint.time();
    *_stream << keypoint.value();
    *_stream << (uint8_t) keypoint.interpolation();
  }
}