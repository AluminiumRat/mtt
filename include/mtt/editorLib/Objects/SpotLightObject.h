#pragma once

#include <mtt/editorLib/Objects/LightWithShadowsObject.h>

namespace mtt
{
  class SpotLightObject : public LightWithShadowsObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitSpotLightObject,
                            visitConstSpotLightObject,
                            LightWithShadowsObject)

    Q_PROPERTY( float angle
                READ angle
                WRITE setAngle
                RESET resetAngle
                NOTIFY angleChanged
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

  signals:
    void angleChanged(float newValue);
    void filterImageChanged(const QString&);

  private:
    float _angle;
    QString _filterImage;
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
}