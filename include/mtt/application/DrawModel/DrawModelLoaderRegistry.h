#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>

#include <QtCore/QString>

#include <mtt/application/DrawModel/AbstractDrawModelLoader.h>
#include <mtt/utilities/Log.h>

namespace mtt
{
  class LogicalDevice;
  class MeshTechniquesFactory;
  class Texture2DLibrary;

  class DrawModelLoaderRegistry
  {
  public:
    DrawModelLoaderRegistry() = default;
    DrawModelLoaderRegistry(const DrawModelLoaderRegistry&) = delete;
    DrawModelLoaderRegistry& operator = (
      const DrawModelLoaderRegistry&) = delete;
    virtual ~DrawModelLoaderRegistry() noexcept = default;

    /// Returns nullptr if no suitable loader is found
    std::unique_ptr<AbstractDrawModelLoader> getLoader(
                                        const QString& filename,
                                        MeshTechniquesFactory& techniqueFactory,
                                        Texture2DLibrary& textureLibrary,
                                        LogicalDevice& device);

    template<typename Loader>
    void registerLoader(const QString& fileExtension);

  private:
    using Builder =
      std::function<std::unique_ptr<AbstractDrawModelLoader>(
                                                        const QString&,
                                                        MeshTechniquesFactory&,
                                                        Texture2DLibrary&,
                                                        LogicalDevice&)>;

  private:
    using BuilderMap = std::map<QString, Builder>;
    BuilderMap _builderMap;
    std::mutex _accessMutex;
  };

  template<typename Loader>
  void DrawModelLoaderRegistry::registerLoader(const QString& fileExtension)
  {
    QString extension = fileExtension.trimmed().toUpper();

    std::lock_guard<std::mutex> lock(_accessMutex);
    if (_builderMap.find(extension) != _builderMap.end())
    {
      Log() << "DrawModelLoaderRegistry::registerLoader: duplicate " << fileExtension.toLocal8Bit().data() << " file extension";
    }
    _builderMap[extension.toUpper()] =
      []( const QString& filename,
          MeshTechniquesFactory& techniqueFactory,
          Texture2DLibrary& textureLibrary,
          LogicalDevice& device) -> std::unique_ptr<AbstractDrawModelLoader>
      {
        return std::make_unique<Loader>(filename,
                                        techniqueFactory,
                                        textureLibrary,
                                        device);
      };
  }
}