#pragma once

#include <mtt/application/Scene/Object.h>
#include <mtt/utilities/SurfaceMaterialData.h>

#include <Objects/OEVisitorExtension.h>

class MaterialObject : public mtt::Object
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension,
                          visitMaterialObject,
                          visitConstMaterialObject,
                          mtt::Object)

  Q_PROPERTY( glm::vec3 albedo
              READ albedo
              WRITE setAlbedo
              NOTIFY albedoChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float roughness
              READ roughness
              WRITE setRoughness
              NOTIFY roughnessChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float specularStrength
              READ specularStrength
              WRITE setSpecularStrength
              NOTIFY specularStrengthChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float metallic
              READ metallic
              WRITE setMetallic
              NOTIFY metallicChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float opaqueFactor
              READ opaqueFactor
              WRITE setOpaqueFactor
              NOTIFY opaqueFactorChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float reflectionFactor
              READ reflectionFactor
              WRITE setReflectionFactor
              NOTIFY reflectionFactorChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( glm::vec3 emissionColor
              READ emissionColor
              WRITE setEmissionColor
              NOTIFY emissionColorChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float emissionFactor
              READ emissionFactor
              WRITE setEmissionFactor
              NOTIFY emissionFactorChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( QString albedoTexture
              READ albedoTexture
              WRITE setAlbedoTexture
              NOTIFY albedoTextureChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( bool useAlphaFromAlbedoTexture
              READ useAlphaFromAlbedoTexture
              WRITE setUseAlphaFromAlbedoTexture
              NOTIFY useAlphaFromAlbedoTextureChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( QString opaqueTexture
              READ opaqueTexture
              WRITE setOpaqueTexture
              NOTIFY opaqueTextureChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( QString specularTexture
              READ specularTexture
              WRITE setSpecularTexture
              NOTIFY specularTextureChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( QString normalTexture
              READ normalTexture
              WRITE setNormalTexture
              NOTIFY normalTextureChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( QString emissionTexture
              READ emissionTexture
              WRITE setEmissionTexture
              NOTIFY emissionTextureChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( QString reflectionTexture
              READ reflectionTexture
              WRITE setReflectionTexture
              NOTIFY reflectionTextureChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  MaterialObject( const QString& name,
                  bool canBeRenamed,
                  const mtt::UID& id = mtt::UID());
  MaterialObject(const MaterialObject&) = delete;
  MaterialObject& operator = (const MaterialObject&) = delete;
  virtual ~MaterialObject() noexcept = default;

  inline const mtt::SurfaceMaterialData& materialData() const noexcept;

  inline const glm::vec3& albedo() const noexcept;
  void setAlbedo(const glm::vec3& newValue) noexcept;

  inline float roughness() const noexcept;
  void setRoughness(float newValue) noexcept;

  inline float specularStrength() const noexcept;
  void setSpecularStrength(float newValue) noexcept;

  inline float metallic() const noexcept;
  void setMetallic(float newValue) noexcept;

  inline float opaqueFactor() const noexcept;
  void setOpaqueFactor(float newValue) noexcept;

  inline float reflectionFactor() const noexcept;
  void setReflectionFactor(float newValue) noexcept;

  inline const glm::vec3& emissionColor() const noexcept;
  void setEmissionColor(const glm::vec3& newValue) noexcept;

  inline float emissionFactor() const noexcept;
  void setEmissionFactor(float newValue) noexcept;

  inline const QString& albedoTexture() const noexcept;
  void setAlbedoTexture(const QString& newValue) noexcept;
  
  inline bool useAlphaFromAlbedoTexture() const noexcept;
  void setUseAlphaFromAlbedoTexture(bool newValue) noexcept;

  inline const QString& opaqueTexture() const noexcept;
  void setOpaqueTexture(const QString & newValue) noexcept;

  inline const QString& specularTexture() const noexcept;
  void setSpecularTexture(const QString& newValue) noexcept;

  inline const QString& normalTexture() const noexcept;
  void setNormalTexture(const QString& newValue) noexcept;

  inline const QString& emissionTexture() const noexcept;
  void setEmissionTexture(const QString& newValue) noexcept;

  inline const QString& reflectionTexture() const noexcept;
  void setReflectionTexture(const QString& newValue) noexcept;

signals:
  void materialDataChanged(const mtt::SurfaceMaterialData& newData);
  void albedoChanged(const glm::vec3& newValue);
  void roughnessChanged(float newValue);
  void specularStrengthChanged(float newValue);
  void metallicChanged(float newValue);
  void opaqueFactorChanged(float newValue);
  void reflectionFactorChanged(float newValue);
  void emissionColorChanged(const glm::vec3& newValue);
  void emissionFactorChanged(float newValue);
  void albedoTextureChanged(const QString& newValue);
  void opaqueTextureChanged(const QString & newValue);
  void useAlphaFromAlbedoTextureChanged(bool newValue);
  void specularTextureChanged(const QString& newValue);
  void normalTextureChanged(const QString& newValue);
  void emissionTextureChanged(const QString& newValue);
  void reflectionTextureChanged(const QString& newValue);

private:
  mtt::SurfaceMaterialData _materialData;
  glm::vec3 _emissionColor;
  float _emissionFactor;
  bool _useAlphaFromAlbedoTexture;

  QString _albedoTexture;
  QString _opaqueTexture;
  QString _specularTexture;
  QString _normalTexture;
  QString _emissionTexture;
  QString _reflectionTexture;
};

inline const mtt::SurfaceMaterialData&
                                  MaterialObject::materialData() const noexcept
{
  return _materialData;
}

inline const glm::vec3& MaterialObject::albedo() const noexcept
{
  return _materialData.albedo;
}

inline float MaterialObject::roughness() const noexcept
{
  return _materialData.roughness;
}

inline float MaterialObject::specularStrength() const noexcept
{
  return _materialData.specularStrength;
}

inline float MaterialObject::metallic() const noexcept
{
  return _materialData.metallic;
}

inline float MaterialObject::opaqueFactor() const noexcept
{
  return _materialData.opaque;
}

inline float MaterialObject::reflectionFactor() const noexcept
{
  return _materialData.reflectionFactor;
}

inline const glm::vec3& MaterialObject::emissionColor() const noexcept
{
  return _emissionColor;
}

inline float MaterialObject::emissionFactor() const noexcept
{
  return _emissionFactor;
}

inline const QString& MaterialObject::albedoTexture() const noexcept
{
  return _albedoTexture;
}

inline bool MaterialObject::useAlphaFromAlbedoTexture() const noexcept
{
  return _useAlphaFromAlbedoTexture;
}

inline const QString& MaterialObject::opaqueTexture() const noexcept
{
  return _opaqueTexture;
}

inline const QString& MaterialObject::specularTexture() const noexcept
{
  return _specularTexture;
}

inline const QString& MaterialObject::normalTexture() const noexcept
{
  return _normalTexture;
}

inline const QString& MaterialObject::emissionTexture() const noexcept
{
  return _emissionTexture;
}

inline const QString& MaterialObject::reflectionTexture() const noexcept
{
  return _reflectionTexture;
}
