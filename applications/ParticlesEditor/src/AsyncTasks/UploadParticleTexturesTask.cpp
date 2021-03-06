#include <QtCore/QObject>

#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/editorLib/EditorApplication.h>

#include <AsyncTasks/UploadParticleTexturesTask.h>

UploadParticleTexturesTask::UploadParticleTexturesTask(
                                const ParticleTextureDescriptions& descriptions,
                                mtt::ParticlesDrawable& drawable) :
  AbstractAsyncTask(QObject::tr("Texture loading"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::SILENT),
  _descriptions(descriptions),
  _drawable(drawable)
{
}

void UploadParticleTexturesTask::asyncPart()
{
  if(_descriptions.empty()) return;

  mtt::LogicalDevice& device = mtt::Application::instance().displayDevice();

  for(const ParticleTextureDescription& description : _descriptions)
  {
    std::shared_ptr<mtt::Texture2D> texture =
                      mtt::Texture2DLibrary::loadTexture( description.filename,
                                                          device,
                                                          true);
    mtt::ParticlesDrawable::TextureData newData;
    newData.texture = texture;
    newData.extent = description.extent;
    _textures.push_back(newData);
  }
}

void UploadParticleTexturesTask::finalizePart()
{
  _drawable.setParticleTextures(_textures);
}