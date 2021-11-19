#pragma once

#include <QtCore/QString>

#include <mtt/Core/AsyncTasks/AbstractAsyncTask.h>

#include <FbxDrawableImporter.h>

class EditorCommonData;

class AddModelFromFbxTask : public mtt::AbstractAsyncTask
{
public:
  AddModelFromFbxTask(
                const QString& filename,
                const mtt::BaseFbxImporter::MaterialOptions& materialOptions,
                EditorCommonData& commonData);
  AddModelFromFbxTask(const AddModelFromFbxTask&) = delete;
  AddModelFromFbxTask& operator = (const AddModelFromFbxTask&) = delete;
  virtual ~AddModelFromFbxTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  QString _filename;
  mtt::BaseFbxImporter::MaterialOptions _materialOptions;
  EditorCommonData& _commonData;

  FbxDrawableImporter::Result _data;
};
