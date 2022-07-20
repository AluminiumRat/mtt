#include <mtt/application/ResourceManager/DrawModelLibrary.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

DrawModelLibrary::DrawModelLibrary(
                      std::unique_ptr<MeshTechniquesFactory> techniqueFactory,
                      Texture2DLibrary& textureLibrary) :
  _techniqueFactory(std::move(techniqueFactory)),
  _textureLibrary(textureLibrary)
{
  if(_techniqueFactory == nullptr) Abort("DrawModelLibrary::DrawModelLibrary: techniqueFactory is null.");
}

std::unique_ptr<SlaveDrawModel> DrawModelLibrary::load( const QString& filename,
                                                        LogicalDevice& device)
{
  DrawModelDescription description;
  description.name = filename;
  description.device = &device;
  std::shared_ptr<MasterDrawModel> masterModel = getOrCreate(description);
  return std::unique_ptr<SlaveDrawModel>(new SlaveDrawModel(masterModel));
}

void DrawModelLibrary::release( const QString& filename,
                                LogicalDevice& device) noexcept
{
  DrawModelDescription description;
  description.name = filename;
  description.device = &device;

  ResourceLibrary::release(description);
}

std::shared_ptr<MasterDrawModel> DrawModelLibrary::buildResource(
                                        const DrawModelDescription& description)
{
  return std::make_shared<MasterDrawModel>( description.name,
                                            *_techniqueFactory,
                                            &_textureLibrary,
                                            *description.device);
}
