#pragma once

#include <memory>

#include <QtCore/QString>

#include <mtt/application/ResourceManager/ResourceLibrary.h>
#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/particles/PSTEffect/PSTEffect.h>

namespace mtt
{
  class Texture2DLibrary;

  struct PSTEffectDescription
  {
    QString name = "";
    LogicalDevice* device = nullptr;

    std::strong_ordering operator <=> (
                          const PSTEffectDescription&) const noexcept = default;
  };

  class PSTEffectLibrary : public ResourceLibrary<PSTDataSource,
                                                  PSTEffectDescription>
  {
  public:
    /// if textureLibrary is nullptr then internal texture library will be
    /// created
    explicit PSTEffectLibrary(Texture2DLibrary* textureLibrary);
    PSTEffectLibrary(const PSTEffectLibrary&) = delete;
    PSTEffectLibrary& operator = (const PSTEffectLibrary&) = delete;
    virtual ~PSTEffectLibrary() noexcept = default;

    /// Thread safe method
    std::unique_ptr<PSTEffect> load(const QString& filename,
                                    LogicalDevice& device);

  protected:
    virtual std::shared_ptr<PSTDataSource> buildResource(
                              const PSTEffectDescription& description) override;

  private:
    Texture2DLibrary* _textureLibrary;
    std::unique_ptr<Texture2DLibrary> _internalTextureLibrary;
  };
}