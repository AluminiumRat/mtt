#pragma once

#include <glm/vec3.hpp>

#include <mtt/editorLib/Objects/RotatableObject.h>

namespace mtt
{
  class CubemapObject;

  class BackgroundObject : public RotatableObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitBackgroundObject,
                            visitConstBackgroundObject,
                            RotatableObject)

    Q_PROPERTY( bool lightEnabled
                READ lightEnabled
                WRITE setLightEnabled
                NOTIFY lightEnabledChanged
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

    Q_PROPERTY( float luminance
                READ luminance
                WRITE setLuminance
                NOTIFY luminanceChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

    Q_PROPERTY( float dissolutionStartDistance
                READ dissolutionStartDistance
                WRITE setDissolutionStartDistance
                NOTIFY dissolutionStartDistanceChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

    Q_PROPERTY( float dissolutionLength
                READ dissolutionLength
                WRITE setDissolutionLength
                NOTIFY dissolutionLengthChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

  public:
    BackgroundObject( const QString& name,
                      bool canBeRenamed,
                      const UID& id = UID());
    BackgroundObject(const BackgroundObject&) = delete;
    BackgroundObject& operator = (const BackgroundObject&) = delete;
    virtual ~BackgroundObject() noexcept = default;

    inline bool lightEnabled() const noexcept;
    void setLightEnabled(bool newValue) noexcept;

    inline float luminance() const noexcept;
    void setLuminance(float newValue) noexcept;

    inline const glm::vec3& color() const noexcept;
    void setColor(const glm::vec3& newValue) noexcept;

    inline float dissolutionStartDistance() const noexcept;
    void setDissolutionStartDistance(float newValue) noexcept;

    inline float dissolutionLength() const noexcept;
    void setDissolutionLength(float newValue) noexcept;

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

  inline float BackgroundObject::luminance() const noexcept
  {
    return _luminance;
  }

  inline const glm::vec3& BackgroundObject::color() const noexcept
  {
    return _color;
  }

  inline float BackgroundObject::dissolutionStartDistance() const noexcept
  {
    return _dissolutionStartDistance;
  }

  inline float BackgroundObject::dissolutionLength() const noexcept
  {
    return _dissolutionLength;
  }

  inline CubemapObject& BackgroundObject::cubemap() noexcept
  {
    return *_cubemap;
  }

  inline const CubemapObject& BackgroundObject::cubemap() const noexcept
  {
    return *_cubemap;
  }
}