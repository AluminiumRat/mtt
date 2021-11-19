#pragma once

#include <glm/vec3.hpp>

#include <Objects/RotatableObject.h>
#include <Objects/OEVisitorExtension.h>

class CubemapObject;

class BackgroundObject : public RotatableObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

  Q_PROPERTY( bool lightEnabled
              READ lightEnabled
              WRITE setLightEnabled
              RESET resetLightEnabled
              NOTIFY lightEnabledChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( glm::vec3 color
              READ color
              WRITE setColor
              RESET resetColor
              NOTIFY colorChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float luminance
              READ luminance
              WRITE setLuminance
              RESET resetLuminance
              NOTIFY luminanceChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float dissolutionStartDistance
              READ dissolutionStartDistance
              WRITE setDissolutionStartDistance
              RESET resetDissolutionStartDistance
              NOTIFY dissolutionStartDistanceChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float dissolutionLength
              READ dissolutionLength
              WRITE setDissolutionLength
              RESET resetDissolutionLength
              NOTIFY dissolutionLengthChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  explicit BackgroundObject(const mtt::UID& id = mtt::UID());
  BackgroundObject(const BackgroundObject&) = delete;
  BackgroundObject& operator = (const BackgroundObject&) = delete;
  virtual ~BackgroundObject() noexcept = default;

  inline bool lightEnabled() const noexcept;
  void setLightEnabled(bool newValue) noexcept;
  inline void resetLightEnabled() noexcept;

  inline float luminance() const noexcept;
  void setLuminance(float newValue) noexcept;
  inline void resetLuminance() noexcept;

  inline const glm::vec3& color() const noexcept;
  void setColor(const glm::vec3& newValue) noexcept;
  inline void resetColor() noexcept;

  inline float dissolutionStartDistance() const noexcept;
  void setDissolutionStartDistance(float newValue) noexcept;
  inline void resetDissolutionStartDistance() noexcept;
  
  inline float dissolutionLength() const noexcept;
  void setDissolutionLength(float newValue) noexcept;
  inline void resetDissolutionLength() noexcept;

  inline CubemapObject& cubemap() noexcept;
  inline const CubemapObject& cubemap() const noexcept;

signals:
  void lightEnabledChanged(bool newValue);
  void luminanceChanged(float newValue);
  void colorChanged(const glm::vec3& newValue);
  void dissolutionStartDistanceChanged(float newValue);
  void dissolutionLengthChanged(float newValue);

private:
  bool _lightEnabled;
  float _luminance;
  glm::vec3 _color;
  float _dissolutionStartDistance;
  float _dissolutionLength;

  CubemapObject* _cubemap;

  QString _xPositiveTexture;
  QString _xNegativeTexture;
  QString _yPositiveTexture;
  QString _yNegativeTexture;
  QString _zPositiveTexture;
  QString _zNegativeTexture;
};

inline bool BackgroundObject::lightEnabled() const noexcept
{
  return _lightEnabled;
}

inline void BackgroundObject::resetLightEnabled() noexcept
{
  setLightEnabled(false);
}

inline float BackgroundObject::luminance() const noexcept
{
  return _luminance;
}

inline void BackgroundObject::resetLuminance() noexcept
{
  setLuminance(1.f);
}

inline const glm::vec3& BackgroundObject::color() const noexcept
{
  return _color;
}

inline void BackgroundObject::resetColor() noexcept
{
  setColor(glm::vec3(1.f));
}

inline float BackgroundObject::dissolutionStartDistance() const noexcept
{
  return _dissolutionStartDistance;
}

inline void BackgroundObject::resetDissolutionStartDistance() noexcept
{
  setDissolutionStartDistance(40.f);
}

inline float BackgroundObject::dissolutionLength() const noexcept
{
  return _dissolutionLength;
}

inline void BackgroundObject::resetDissolutionLength() noexcept
{
  setDissolutionLength(10.f);
}

inline CubemapObject& BackgroundObject::cubemap() noexcept
{
  return *_cubemap;
}

inline const CubemapObject& BackgroundObject::cubemap() const noexcept
{
  return *_cubemap;
}
