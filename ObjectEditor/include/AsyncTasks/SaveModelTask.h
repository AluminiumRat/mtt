#pragma once

#include <string>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/Application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/Application/DataStream.h>

namespace mtt
{
  class Object;
}

class QFile;

class EditorCommonData;
class EditorScene;

class SaveModelTask : public mtt::AbstractAsyncTask
{
public:
  static const inline std::string fileHead = "MMDModelFile";
  static constexpr uint32_t fileVersion = 0;

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

public:
  SaveModelTask(const EditorScene& scene,
                const QString& filename,
                EditorCommonData& commonData);
  SaveModelTask(const SaveModelTask&) = delete;
  SaveModelTask& operator = (const SaveModelTask&) = delete;
  virtual ~SaveModelTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  struct ObjectIdSaver;
  struct ObjectDataSaver;

private:
  void _writeHead();
  static void _saveObject(const mtt::Object& object,
                          mtt::DataStream& stream,
                          const QDir& fileDirectory);
  void _writeMaterials();
  void _writeSkeletons();
  void _writeGeometry();
  void _writeAnimations();

private:
  const EditorScene& _scene;
  QString _filename;
  QFile* _file;
  mtt::DataStream* _stream;
  QDir _fileDirectory;
  EditorCommonData& _commonData;
};