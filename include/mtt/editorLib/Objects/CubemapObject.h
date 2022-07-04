#pragma once

#include <array>

#include <mtt/application/Scene/Object.h>
#include <mtt/editorLib/Objects/CEVisitorExtension.h>

namespace mtt
{
  class CubemapObject : public Object
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitCubemapObject,
                            visitConstCubemapObject,
                            Object)

    Q_PROPERTY( std::array<QString, 6> textures
                READ textures
                WRITE setTextures
                NOTIFY texturesChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

  public:
    using Textures = std::array<QString, 6>;

    enum Side
    {
      SIDE_X_POSITIVE = 0,
      SIDE_X_NEGATIVE = 1,
      SIDE_Y_POSITIVE = 2,
      SIDE_Y_NEGATIVE = 3,
      SIDE_Z_NEGATIVE = 4,
      SIDE_Z_POSITIVE = 5
    };

  public:
    CubemapObject(const QString& name,
                  bool canBeRenamed,
                  const UID& id = UID());
    CubemapObject(const CubemapObject&) = delete;
    CubemapObject& operator = (const CubemapObject&) = delete;
    virtual ~CubemapObject() noexcept = default;

    inline const Textures& textures() const noexcept;
    inline const QString& sideTexture(Side side) const noexcept;
    void setTextures(const Textures& newValue) noexcept;

  signals:
    void texturesChanged(const Textures& newValue);

  private:
    std::array<QString, 6> _textures;
  };

  inline const CubemapObject::Textures& CubemapObject::textures() const noexcept
  {
    return _textures;
  }

  inline const QString& CubemapObject::sideTexture(Side side) const noexcept
  {
    return _textures[side];
  }
}