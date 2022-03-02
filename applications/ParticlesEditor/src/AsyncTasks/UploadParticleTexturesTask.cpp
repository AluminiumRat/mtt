#include <QtCore/QObject>

#include <mtt/editorLib/EditorApplication.h>

#include <AsyncTasks/UploadParticleTexturesTask.h>
#include <Render/ParticlesDrawable.h>

UploadParticleTexturesTask::UploadParticleTexturesTask(
                                        const std::vector<QString>& filenames,
                                        ParticlesDrawable& drawable) :
  AbstractAsyncTask(QObject::tr("Texture loading"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::SILENT),
  _filenames(filenames),
  _drawable(drawable)
{
}

void UploadParticleTexturesTask::asyncPart()
{
  if(_filenames.empty()) return;

  mtt::Texture2DLibrary& textureLibrary =
                              mtt::EditorApplication::instance().textureLibrary;
  mtt::LogicalDevice& device = mtt::Application::instance().displayDevice();

  for(const QString& filename : _filenames)
  {
    std::shared_ptr<mtt::Texture2D> texture = textureLibrary.load(filename,
                                                                  device,
                                                                  true);
    _textures.push_back(texture);
  }
}

void UploadParticleTexturesTask::finalizePart()
{
  _drawable.setParticleTextures(_textures);
}