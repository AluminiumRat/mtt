#include <mtt/particles/PSTEffect/PSTEffectLibrary.h>

using namespace mtt;

PSTEffectLibrary::PSTEffectLibrary(Texture2DLibrary* textureLibrary) :
  _textureLibrary(textureLibrary)
{
  if (_textureLibrary == nullptr)
  {
    _internalTextureLibrary.reset(new Texture2DLibrary());
    _textureLibrary = _internalTextureLibrary.get();
  }
}

std::unique_ptr<PSTEffect> PSTEffectLibrary::load(const QString& filename,
                                                  LogicalDevice& device)
{
  PSTEffectDescription description;
  description.name = filename;
  description.device = &device;
  std::shared_ptr<PSTDataSource> dataSource = getOrCreate(description);
  return std::make_unique<PSTEffect>(dataSource);
}

std::shared_ptr<PSTDataSource> PSTEffectLibrary::buildResource(
                                        const PSTEffectDescription& description)
{
  return std::make_shared<PSTDataSource>( description.name,
                                          _textureLibrary,
                                          *description.device);
}
