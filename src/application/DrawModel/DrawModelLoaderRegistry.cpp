#include <QtCore/QFileInfo>

#include <mtt/application/DrawModel/DrawModelLoaderRegistry.h>

using namespace mtt;

std::unique_ptr<AbstractDrawModelLoader>
                  DrawModelLoaderRegistry::getLoader(
                                        const QString& filename,
                                        MeshTechniquesFactory& techniqueFactory,
                                        Texture2DLibrary& textureLibrary,
                                        LogicalDevice& device)
{
  QFileInfo fileInfo(filename);
  QString extension = fileInfo.suffix().trimmed().toUpper();

  BuilderMap::iterator iBuilder = _builderMap.find(extension);
  if(iBuilder == _builderMap.end()) return nullptr;
  Builder& builder = iBuilder->second;
  return builder(filename, techniqueFactory, textureLibrary, device);
}
