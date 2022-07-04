#pragma once

#include <mtt/editorLib/Objects/FadingLightObject.h>

namespace mtt
{
  class SpotLightObject : public FadingLightObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitSpotLightObject,
                            visitConstSpotLightObject,
                            FadingLightObject)

    Q_PROPERTY( float angle
                READ angle
                WRITE setAngle
                NOTIFY angleChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

    Q_PROPERTY( QString filterImage
                READ filterImage
                WRITE setFilterImage
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

    inline const QString& filterImage() const noexcept;
    virtual void setFilterImage(const QString& newValue) noexcept;

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

  inline const QString& SpotLightObject::filterImage() const noexcept
  {
    return _filterImage;
  }
}