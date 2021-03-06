#pragma once

#include <glm/vec3.hpp>

#include <mtt/editorLib/Objects/EnvironmentObject.h>

namespace mtt
{
  class LightObject : public EnvironmentObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitLightObject,
                            visitConstLightObject,
                            EnvironmentObject)

    Q_PROPERTY( bool enabled
                READ enabled
                WRITE setEnabled
                NOTIFY enabledChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

    Q_PROPERTY( float distance
                READ distance
                WRITE setDistance
                NOTIFY distanceChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

    Q_PROPERTY( glm::vec3 color
                READ color
                WRITE setColor
                NOTIFY colorChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

    Q_PROPERTY( float baseIlluminance
                READ baseIlluminance
                WRITE setBaseIlluminance
                NOTIFY baseIlluminanceChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)
  public:
    LightObject(const QString& name,
                bool canBeRenamed,
                const UID = UID());
    LightObject(const LightObject&) = delete;
    LightObject& operator = (const LightObject&) = delete;
    virtual ~LightObject() noexcept = default;

    inline bool enabled() const noexcept;
    virtual void setEnabled(bool newValue) noexcept;

    inline float distance() const noexcept;
    virtual void setDistance(float newValue) noexcept;

    inline const glm::vec3& color() const noexcept;
    virtual void setColor(const glm::vec3& newValue) noexcept;

    /// Illuminance at half of distance
    inline float baseIlluminance() const noexcept;
    virtual void setBaseIlluminance(float newValue) noexcept;

  signals:
    void enabledChanged(bool newValue);
    void distanceChanged(float newValue);
    void colorChanged(const glm::vec3& newValue);
    void baseIlluminanceChanged(float newValue);

  private:
    bool _enabled;
    float _distance;
    glm::vec3 _color;
    float _baseIlluminance;
  };

  inline bool LightObject::enabled() const noexcept
  {
    return _enabled;
  }

  inline float LightObject::distance() const noexcept
  {
    return _distance;
  }

  inline const glm::vec3& LightObject::color() const noexcept
  {
    return _color;
  }

  inline float LightObject::baseIlluminance() const noexcept
  {
    return _baseIlluminance;
  }
}