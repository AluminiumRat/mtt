#pragma once

#include <mtt/editorLib/Objects/LightObject.h>

namespace mtt
{
  class DirectLightObject : public LightObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitDirectLightObject,
                            visitConstDirectLightObject,
                            LightObject)

    Q_PROPERTY( float radius
                READ radius
                WRITE setRadius
                RESET resetRadius
                NOTIFY radiusChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

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

    Q_PROPERTY( size_t cascadeSize
                READ cascadeSize
                WRITE setCascadeSize
                RESET resetCascadeSize
                NOTIFY cascadeSizeChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

    Q_PROPERTY( float blurSize
                READ blurSize
                WRITE setBlurSize
                RESET resetBlurSize
                NOTIFY blurSizeChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)
  public:
    DirectLightObject(const QString& name,
                      bool canBeRenamed,
                      const UID = UID());
    DirectLightObject(const DirectLightObject&) = delete;
    DirectLightObject& operator = (const DirectLightObject&) = delete;
    virtual ~DirectLightObject() noexcept = default;

    inline float radius() const noexcept;
    void setRadius(float newValue) noexcept;
    inline void resetRadius() noexcept;

    inline bool shadowsEnabled() const noexcept;
    virtual void setShadowsEnabled(bool newValue) noexcept;
    inline void resetShadowsEnabled() noexcept;

    inline size_t shadowmapSize() const noexcept;
    virtual void setShadowmapSize(size_t newValue) noexcept;
    inline void resetShadowmapSize() noexcept;

    inline size_t cascadeSize() const noexcept;
    virtual void setCascadeSize(size_t newValue) noexcept;
    inline void resetCascadeSize() noexcept;

    /// blur size in meters
    inline float blurSize() const noexcept;
    virtual void setBlurSize(float newValue) noexcept;
    inline void resetBlurSize() noexcept;

  signals:
    void radiusChanged(float newValue);
    void shadowsEnabledChanged(bool newValue);
    void shadowmapSizeChanged(size_t newValue);
    void cascadeSizeChanged(size_t newValue);
    void blurSizeChanged(size_t newValue);

  private:
    float _radius;
    bool _shadowsEnabled;
    size_t _shadowmapSize;
    size_t _cascadeSize;
    float _blurSize;
  };

  inline float DirectLightObject::radius() const noexcept
  {
    return _radius;
  }

  inline void DirectLightObject::resetRadius() noexcept
  {
    setRadius(10);
  }

  inline bool DirectLightObject::shadowsEnabled() const noexcept
  {
    return _shadowsEnabled;
  }

  inline void DirectLightObject::resetShadowsEnabled() noexcept
  {
    setShadowsEnabled(false);
  }

  inline size_t DirectLightObject::shadowmapSize() const noexcept
  {
    return _shadowmapSize;
  }

  inline void DirectLightObject::resetShadowmapSize() noexcept
  {
    setShadowmapSize(256);
  }

  inline size_t DirectLightObject::cascadeSize() const noexcept
  {
    return _cascadeSize;
  }

  inline void DirectLightObject::resetCascadeSize() noexcept
  {
    setCascadeSize(1);
  }

  inline float DirectLightObject::blurSize() const noexcept
  {
    return _blurSize;
  }

  inline void DirectLightObject::resetBlurSize() noexcept
  {
    setBlurSize(0.f);
  }
}