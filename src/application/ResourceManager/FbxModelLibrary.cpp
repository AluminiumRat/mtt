#include <mtt/application/DrawModel/FbxModelLoader.h>
#include <mtt/application/ResourceManager/FbxModelLibrary.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

FbxModelLibrary::FbxModelLibrary(
                      std::unique_ptr<MeshTechniquesFactory> techniqueFactory,
                      Texture2DLibrary& textureLibrary) :
  _techniqueFactory(std::move(techniqueFactory)),
  _textureLibrary(textureLibrary)
{
  if(_techniqueFactory == nullptr) Abort("FbxModelLibrary::FbxModelLibrary: techniqueFactory is null.");
}

std::unique_ptr<SlaveDrawModel> FbxModelLibrary::load(
                              const QString& filename,
                              BaseFbxImporter::MaterialOptions materialOptions,
                              LogicalDevice& device)
{
  FbxModelDescription description;
  description.name = filename;
  description.materialOptions = materialOptions;
  description.device = &device;
  std::shared_ptr<MasterDrawModel> masterModel = getOrCreate(description);
  return std::unique_ptr<SlaveDrawModel>(new SlaveDrawModel(masterModel));
}

std::shared_ptr<MasterDrawModel> FbxModelLibrary::buildResource(
                                        const FbxModelDescription& description)
{
  FbxModelLoader loader(description.name,
                        description.materialOptions,
                        *_techniqueFactory,
                        _textureLibrary,
                        *description.device);
  return loader.load();
}
