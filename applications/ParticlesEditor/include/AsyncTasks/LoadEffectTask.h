#pragma once

#include <memory>
#include <vector>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/application/DataStream.h>

#include <Objects/HierarhicalObject.h>
#include <Objects/ParticleAnimation.h>
#include <Objects/ParticleField.h>

class QFile;

class EditorCommonData;
class EditorScene;

class LoadEffectTask : public mtt::AbstractAsyncTask
{
public:
  LoadEffectTask( EditorScene& scene,
                  const QString& filename,
                  EditorCommonData& commonData);
  LoadEffectTask(const LoadEffectTask&) = delete;
  LoadEffectTask& operator = (const LoadEffectTask&) = delete;
  virtual ~LoadEffectTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  void _checkHead();
  void _loadModificators();
  void _clearScene() noexcept;

private:
  EditorScene& _scene;
  QString _filename;
  QFile* _file;
  mtt::DataStream* _stream;
  QDir _fileDirectory;
  EditorCommonData& _commonData;
  mtt::UID::ValueType _mixUIDValue;

  std::unique_ptr<ParticleField> _field;
  std::vector<std::unique_ptr<HierarhicalObject>> _modificators;
  std::unique_ptr<ParticleAnimation> _animation;
};