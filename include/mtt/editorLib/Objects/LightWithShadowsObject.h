#pragma once

#include <mtt/editorLib/Objects/LightObject.h>

namespace mtt
{
  class LightWithShadowsObject : public LightObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitLightWithShadowsObject,
                            visitConstLightWithShadowsObject,
                            LightObject)

    Q_PROPERTY( bool shadowsEnabled
                READ shadowsEnabled
                WRITE setShadowsEnabled
                RESET resetShadowsEnabled
                NOTIFY shadowsEnabledChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

    Q_PROPERTY( size_t shadowmapSize
                READ shadowmapSize
                WRITE setShadowmapSize
                RESET resetShadowmapSize
                NOTIFY shadowmapSizeChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

    Q_PROPERTY( float shadowBlur
                READ shadowBlur
                WRITE setShadowBlur
                RESET resetShadowBlur
                NOTIFY shadowBlurChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

  public:
    LightWithShadowsObject( const QString& name,
                            bool canBeRenamed,
                            const UID = UID());
    LightWithShadowsObject(const LightWithShadowsObject&) = delete;
    LightWithShadowsObject& operator = (const LightWithShadowsObject&) = delete;
    virtual ~LightWithShadowsObject() noexcept = default;

    inline bool shadowsEnabled() const noexcept;
    virtual void setShadowsEnabled(bool newValue) noexcept;
    inline void resetShadowsEnabled() noexcept;

    inline size_t shadowmapSize() const noexcept;
    virtual void setShadowmapSize(size_t newValue) noexcept;
    inline void resetShadowmapSize() noexcept;

    /// blur size in radians or meters
    inline float shadowBlur() const noexcept;
    virtual void setShadowBlur(float newValue) noexcept;
    inline void resetShadowBlur() noexcept;

  signals:
    void shadowsEnabledChanged(bool newValue);
    void shadowmapSizeChanged(size_t newValue);
    void shadowBlurChanged(size_t newValue);

  private:
    bool _shadowsEnabled;
    size_t _shadowmapSize;
    float _shadowBlur;
  };

  inline bool LightWithShadowsObject::shadowsEnabled() const noexcept
  {
    return _shadowsEnabled;
  }

  inline void LightWithShadowsObject::resetShadowsEnabled() noexcept
  {
    setShadowsEnabled(false);
  }

  inline size_t LightWithShadowsObject::shadowmapSize() const noexcept
  {
    return _shadowmapSize;
  }

  inline void LightWithShadowsObject::resetShadowmapSize() noexcept
  {
    setShadowmapSize(256);
  }

  inline float LightWithShadowsObject::shadowBlur() const noexcept
  {
    return _shadowBlur;
  }

  inline void LightWithShadowsObject::resetShadowBlur() noexcept
  {
    setShadowBlur(0.f);
  }
}