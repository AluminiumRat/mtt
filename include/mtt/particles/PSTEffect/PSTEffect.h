#pragma once

#include <memory>

#include <mtt/application/TimeT.h>
#include <mtt/particles/Drawable/ParticlesDrawable.h>
#include <mtt/particles/PSTEffect/PSTDataSource.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

namespace mtt
{
  class PSTEffect : public SimpleDrawableNode
  {
  public:
    /// If textureLibrary is nullptr then textures will be loaded without
    /// resource management
    PSTEffect(const QString& filename,
              Texture2DLibrary* textureLibrary,
              LogicalDevice& device);
    PSTEffect(const PSTEffect&) = delete;
    PSTEffect& operator = (const PSTEffect&) = delete;
    virtual ~PSTEffect() noexcept = default;

    inline TimeT duration() const noexcept;
    inline TimeT currentTime() const noexcept;
    void setTime(TimeT newValue) noexcept;

    inline ParticlesDrawable::LightingType lightingType() const noexcept;
    inline void setLightingType(
                            ParticlesDrawable::LightingType newValue) noexcept;

  protected:
    virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

  private:
    std::shared_ptr<PSTDataSource> _dataSource;
    ParticlesDrawable _drawable;
    TimeT _currentTime;
    bool _needUpdate;
  };

  inline TimeT PSTEffect::duration() const noexcept
  {
    return _dataSource->duration();
  }

  inline TimeT PSTEffect::currentTime() const noexcept
  {
    return _currentTime;
  }

  inline ParticlesDrawable::LightingType
                                      PSTEffect::lightingType() const noexcept
  {
    return _drawable.lightingType();
  }
  
  inline void PSTEffect::setLightingType(
                            ParticlesDrawable::LightingType newValue) noexcept
  {
    _drawable.setLightingType(newValue);
  }
}