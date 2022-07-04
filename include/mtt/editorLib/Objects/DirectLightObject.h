#pragma once

#include <mtt/editorLib/Objects/LightWithShadowsObject.h>

namespace mtt
{
  class DirectLightObject : public LightWithShadowsObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitDirectLightObject,
                            visitConstDirectLightObject,
                            LightObject)

    Q_PROPERTY( float shadowDistance
                READ shadowDistance
                WRITE setShadowDistance
                NOTIFY shadowDistanceChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

    Q_PROPERTY( size_t cascadeSize
                READ cascadeSize
                WRITE setCascadeSize
                NOTIFY cascadeSizeChanged
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

    inline float shadowDistance() const noexcept;
    void setShadowDistance(float newValue) noexcept;

    inline size_t cascadeSize() const noexcept;
    virtual void setCascadeSize(size_t newValue) noexcept;

  signals:
    void shadowDistanceChanged(float newValue);
    void cascadeSizeChanged(size_t newValue);

  private:
    float _shadowDistance;
    size_t _cascadeSize;
  };

  inline float DirectLightObject::shadowDistance() const noexcept
  {
    return _shadowDistance;
  }

  inline size_t DirectLightObject::cascadeSize() const noexcept
  {
    return _cascadeSize;
  }
}