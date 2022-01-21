#pragma once

#include <memory>
#include <vector>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/application/DataStream.h>

#include <Objects/AnimationObject.h>
#include <Objects/LODObject.h>
#include <Objects/MaterialObject.h>
#include <Objects/SkeletonObject.h>

class QFile;

class EditorCommonData;
class EditorScene;

class LoadModelTask : public mtt::AbstractAsyncTask
{
public:
  LoadModelTask(EditorScene& scene,
                const QString& filename,
                EditorCommonData& commonData);
  LoadModelTask(const LoadModelTask&) = delete;
  LoadModelTask& operator = (const LoadModelTask&) = delete;
  virtual ~LoadModelTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  void _checkHead();
  void _loadMaterials();
  void _loadSkeletons();
  void _loadGeometry();
  void _loadAnimations();
  void _clearScene() noexcept;

private:
  EditorScene& _scene;
  QString _filename;
  QFile* _file;
  mtt::DataStream* _stream;
  QDir _fileDirectory;
  EditorCommonData& _commonData;
  mtt::UID::ValueType _mixUIDValue;

  std::vector<std::unique_ptr<AnimationObject>> _animations;
  std::vector<std::unique_ptr<LODObject>> _lods;
  std::vector<std::unique_ptr<MaterialObject>> _materials;
  std::vector<std::unique_ptr<SkeletonObject>> _skeletons;
};