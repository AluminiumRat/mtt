#pragma once

#include <mtt/editorLib/Objects/LightObject.h>

namespace mtt
{
  class CubemapObject;

  class AmbientLightObject : public LightObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitAmbientLightObject,
                            visitConstAmbientLightObject,
                            LightObject)

    Q_PROPERTY( float saturationDistance
                READ saturationDistance
                WRITE setSaturationDistance
                NOTIFY saturationDistanceChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

  public:
    AmbientLightObject( const QString& name,
                        bool canBeRenamed,
                        const UID = UID());
    AmbientLightObject(const AmbientLightObject&) = delete;
    AmbientLightObject& operator = (const AmbientLightObject&) = delete;
    virtual ~AmbientLightObject() noexcept = default;

    inline float saturationDistance() const noexcept;
    virtual void setSaturationDistance(float newValue) noexcept;

    inline CubemapObject& ambientMap() noexcept;
    inline const CubemapObject& ambientMap() const noexcept;

  signals:
    void saturationDistanceChanged(float newValue);

  private:
    float _saturationDistance;
    CubemapObject* _ambientMap;
  };

  inline float AmbientLightObject::saturationDistance() const noexcept
  {
    return _saturationDistance;
  }

  inline CubemapObject& AmbientLightObject::ambientMap() noexcept
  {
    return *_ambientMap;
  }

  inline const CubemapObject& AmbientLightObject::ambientMap() const noexcept
  {
    return *_ambientMap;
  }
}