#include <stdexcept>

#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/SaveEffectTask.h>
#include <Objects/ObjectSaver.h>
#include <Objects/PEEObjectFactory.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

SaveEffectTask::SaveEffectTask( const EditorScene& scene,
                                const QString& filename,
                                EditorCommonData& commonData) :
  AbstractAsyncTask(QObject::tr("Saving"),
                    mtt::AbstractAsyncTask::DEPENDENT,
                    mtt::AbstractAsyncTask::EXPLICIT),
  _scene(scene),
  _filename(filename),
  _file(nullptr),
  _fileDirectory(QFileInfo(_filename).dir()),
  _stream(nullptr),
  _commonData(commonData)
{
}

void SaveEffectTask::asyncPart()
{
  if(!_fileDirectory.exists()) throw std::runtime_error("The file directory does not exist");

  QFile file(_filename);
  if(!file.open(QFile::WriteOnly)) throw std::runtime_error("Unable to open model file");
  _file = &file;

  mtt::DataStream stream(&file);
  _stream = &stream;

  _writeHead();

  ObjectSaver saver;
  PEEObjectFactory factory;

  saver.saveObject( _scene.root().particleField(),
                    *_stream,
                    _fileDirectory,
                    factory);

  _writeModificators();

  saver.saveObject( _scene.root().animation(),
                    *_stream,
                    _fileDirectory,
                    factory);
}

void SaveEffectTask::_writeHead()
{
  _file->write(fileHead.c_str(), fileHead.length());
  *_stream << fileVersion;
}

void SaveEffectTask::_writeModificators()
{
  ObjectSaver saver;
  PEEObjectFactory factory;

  uint32_t modificatorsNumber =
                              _scene.root().modificatorsGroup().childsNumber();
  *_stream << modificatorsNumber;
  for(uint32_t modificatorIndex = 0;
      modificatorIndex < modificatorsNumber;
      modificatorIndex++)
  {
    saver.saveObject( _scene.root().modificatorsGroup().child(modificatorIndex),
                      *_stream,
                      _fileDirectory,
                      factory);
  }
}

void SaveEffectTask::finalizePart()
{
  _commonData.setEffectFilename(_filename);
}
