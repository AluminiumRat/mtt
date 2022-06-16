#pragma once

#include <mtt/editorLib/Objects/LightWithShadowsObject.h>

namespace mtt
{
  class CubemapObject;

  class PointLightObject : public LightWithShadowsObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitPointLightObject,
                            visitConstPointLightObject,
                            LightWithShadowsObject)
  public:
    PointLightObject(const QString& name,
                    bool canBeRenamed,
                    const UID = UID());
    PointLightObject(const PointLightObject&) = delete;
    PointLightObject& operator = (const PointLightObject&) = delete;
    virtual ~PointLightObject() noexcept = default;

    inline CubemapObject& filterCubemap() noexcept;
    inline const CubemapObject& filterCubemap() const noexcept;

  private:
    CubemapObject* _filterCubemap;
  };

  inline CubemapObject& PointLightObject::filterCubemap() noexcept
  {
    return *_filterCubemap;
  }

  inline const CubemapObject& PointLightObject::filterCubemap() const noexcept
  {
    return *_filterCubemap;
  }
}