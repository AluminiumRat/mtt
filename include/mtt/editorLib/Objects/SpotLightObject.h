#pragma once

#include <mtt/editorLib/Objects/LightObject.h>

namespace mtt
{
  class SpotLightObject : public LightObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitSpotLightObject,
                            visitConstSpotLightObject,
                            LightObject)

    Q_PROPERTY( float angle
                READ angle
                WRITE setAngle
                RESET resetAngle
                NOTIFY angleChanged
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

    Q_PROPERTY( float blurAngle
                READ blurAngle
                WRITE setBlurAngle
                RESET resetBlurAngle
                NOTIFY blurAngleChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

    Q_PROPERTY( QString filterImage
                READ filterImage
                WRITE setFilterImage
                RESET resetFilterImage
                NOTIFY filterImageChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

  public:
    SpotLightObject(const QString& name,
                    bool canBeRenamed,
                    const UID = UID());
    SpotLightObject(const SpotLightObject&) = delete;
    SpotLightObject& operator = (const SpotLightObject&) = delete;
    virtual ~SpotLightObject() noexcept = default;

    inline float angle() const noexcept;
    void setAngle(float newValue) noexcept;
    inline void resetAngle() noexcept;

    inline const QString& filterImage() const noexcept;
    virtual void setFilterImage(const QString& newValue) noexcept;
    inline void resetFilterImage() noexcept;

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
    void angleChanged(float newValue);
    void shadowsEnabledChanged(bool newValue);
    void shadowmapSizeChanged(size_t newValue);
    void blurAngleChanged(size_t newValue);
    void filterImageChanged(const QString&);

  private:
    float _angle;
    QString _filterImage;
    bool _shadowsEnabled;
    size_t _shadowmapSize;
    float _blurAngle;
  };

  inline float SpotLightObject::angle() const noexcept
  {
    return _angle;
  }

  inline void SpotLightObject::resetAngle() noexcept
  {
    setAngle(glm::pi<float>() / 4.f);
  }

  inline const QString& SpotLightObject::filterImage() const noexcept
  {
    return _filterImage;
  }

  inline void SpotLightObject::resetFilterImage() noexcept
  {
    setFilterImage(QString());
  }

  inline bool SpotLightObject::shadowsEnabled() const noexcept
  {
    return _shadowsEnabled;
  }

  inline void SpotLightObject::resetShadowsEnabled() noexcept
  {
    setShadowsEnabled(false);
  }

  inline size_t SpotLightObject::shadowmapSize() const noexcept
  {
    return _shadowmapSize;
  }

  inline void SpotLightObject::resetShadowmapSize() noexcept
  {
    setShadowmapSize(256);
  }

  inline float SpotLightObject::blurAngle() const noexcept
  {
    return _blurAngle;
  }

  inline void SpotLightObject::resetBlurAngle() noexcept
  {
    setBlurAngle(0.f);
  }
}