#pragma once

#include <Objects/LightObject.h>

class AmbientLightObject : public LightObject
{
  Q_OBJECT

  Q_PROPERTY( float saturationDistance
              READ saturationDistance
              WRITE setSaturationDistance
              RESET resetSaturationDistance
              NOTIFY saturationDistanceChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  explicit AmbientLightObject(const mtt::UID = mtt::UID());
  AmbientLightObject(const AmbientLightObject&) = delete;
  AmbientLightObject& operator = (const AmbientLightObject&) = delete;
  virtual ~AmbientLightObject() noexcept = default;

  inline float saturationDistance() const noexcept;
  virtual void setSaturationDistance(float newValue) noexcept;
  inline void resetSaturationDistance() noexcept;

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

inline void AmbientLightObject::resetSaturationDistance() noexcept
{
  setSaturationDistance(0);
}

inline CubemapObject& AmbientLightObject::ambientMap() noexcept
{
  return *_ambientMap;
}

inline const CubemapObject& AmbientLightObject::ambientMap() const noexcept
{
  return *_ambientMap;
}
