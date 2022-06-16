#pragma once

#include <mtt/editorLib/Objects/LightObject.h>

namespace mtt
{
  class CubemapObject;

  class PointLightObject : public LightObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitPointLightObject,
                            visitConstPointLightObject,
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

    Q_PROPERTY( float blurAngle
                READ blurAngle
                WRITE setBlurAngle
                RESET resetBlurAngle
                NOTIFY blurAngleChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

  public:
    PointLightObject(const QString& name,
                    bool canBeRenamed,
                    const UID = UID());
    PointLightObject(const PointLightObject&) = delete;
    PointLightObject& operator = (const PointLightObject&) = delete;
    virtual ~PointLightObject() noexcept = default;

    inline CubemapObject& filterCubemap() noexcept;
    inline const CubemapObject& filterCubemap() const noexcept;

    inline bool shadowsEnabled() const noexcept;
    virtual void setShadowsEnabled(bool newValue) noexcept;
    inline void resetShadowsEnabled() noexcept;

    inline size_t shadowmapSize() const noexcept;
    virtual void setShadowmapSize(size_t newValue) noexcept;
    inline void resetShadowmapSize() noexcept;

    /// blur size in radians
    inline float blurAngle() const noexcept;
    virtual void setBlurAngle(float newValue) noexcept;
    inline void resetBlurAngle() noexcept;

  signals:
    void shadowsEnabledChanged(bool newValue);
    void shadowmapSizeChanged(size_t newValue);
    void blurAngleChanged(size_t newValue);
    void filterImageChanged(const QString&);

  private:
    CubemapObject* _filterCubemap;

    bool _shadowsEnabled;
    size_t _shadowmapSize;
    float _blurAngle;
  };

  inline CubemapObject& PointLightObject::filterCubemap() noexcept
  {
    return *_filterCubemap;
  }

  inline const CubemapObject& PointLightObject::filterCubemap() const noexcept
  {
    return *_filterCubemap;
  }

  inline bool PointLightObject::shadowsEnabled() const noexcept
  {
    return _shadowsEnabled;
  }

  inline void PointLightObject::resetShadowsEnabled() noexcept
  {
    setShadowsEnabled(false);
  }

  inline size_t PointLightObject::shadowmapSize() const noexcept
  {
    return _shadowmapSize;
  }

  inline void PointLightObject::resetShadowmapSize() noexcept
  {
    setShadowmapSize(256);
  }

  inline float PointLightObject::blurAngle() const noexcept
  {
    return _blurAngle;
  }

  inline void PointLightObject::resetBlurAngle() noexcept
  {
    setBlurAngle(0.f);
  }
}