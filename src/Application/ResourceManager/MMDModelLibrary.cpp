#include <mtt/Application/DrawModel/MMDModelLoader.h>
#include <mtt/Application/ResourceManager/MMDModelLibrary.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

MMDModelLibrary::MMDModelLibrary(
                      std::unique_ptr<MeshTechniquesFactory> techniqueFactory,
                      Texture2DLibrary& textureLibrary) :
  _techniqueFactory(std::move(techniqueFactory)),
  _textureLibrary(textureLibrary)
{
  if(_techniqueFactory == nullptr) Abort("MMDModelLibrary::MMDModelLibrary: techniqueFactory is null.");
}

std::unique_ptr<SlaveDrawModel> MMDModelLibrary::load(const QString& filename,
                                                      LogicalDevice& device)
{
  MMDModelDescription description;
  description.name = filename;
  description.device = &device;
  std::shared_ptr<MasterDrawModel> masterModel = getOrCreate(description);
  return std::unique_ptr<SlaveDrawModel>(new SlaveDrawModel(masterModel));
}

std::shared_ptr<MasterDrawModel> MMDModelLibrary::buildResource(
                                        const MMDModelDescription& description)
{
  MMDModelLoader loader(description.name,
                        *_techniqueFactory,
                        _textureLibrary,
                        *description.device);
  return loader.load();
}
