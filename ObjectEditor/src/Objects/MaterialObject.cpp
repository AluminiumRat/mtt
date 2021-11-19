#include <Objects/MaterialObject.h>

MaterialObject::MaterialObject(const mtt::UID& id) :
  Object(id),
  _emissionColor(1),
  _emissionFactor(0),
  _useAlphaFromAlbedoTexture(false)
{
  _materialData.albedo = glm::vec3(1);
  _materialData.roughness = 1;
  _materialData.specularStrength = 1;
  _materialData.metallic = 0;
  _materialData.reflectionFactor = 0;
  _materialData.emission = glm::vec3(0);
}

void MaterialObject::setAlbedo(const glm::vec3& newValue) noexcept
{
  if(_materialData.albedo == newValue) return;
  _materialData.albedo = newValue;
  emit materialDataChanged(_materialData);
  emit albedoChanged(newValue);
}

void MaterialObject::setRoughness(float newValue) noexcept
{
  if (_materialData.roughness == newValue) return;
  _materialData.roughness = newValue;
  emit materialDataChanged(_materialData);
  emit roughnessChanged(newValue);
}

void MaterialObject::setSpecularStrength(float newValue) noexcept
{
  if (_materialData.specularStrength == newValue) return;
  _materialData.specularStrength = newValue;
  emit materialDataChanged(_materialData);
  emit specularStrengthChanged(newValue);
}

void MaterialObject::setMetallic(float newValue) noexcept
{
  if (_materialData.metallic == newValue) return;
  _materialData.metallic = newValue;
  emit materialDataChanged(_materialData);
  emit metallicChanged(newValue);
}

void MaterialObject::setOpaqueFactor(float newValue) noexcept
{
  if (_materialData.opaque == newValue) return;
  _materialData.opaque = newValue;
  emit materialDataChanged(_materialData);
  emit opaqueFactorChanged(newValue);
}

void MaterialObject::setReflectionFactor(float newValue) noexcept
{
  if (_materialData.reflectionFactor == newValue) return;
  _materialData.reflectionFactor = newValue;
  emit materialDataChanged(_materialData);
  emit reflectionFactorChanged(newValue);
}

void MaterialObject::setEmissionColor(const glm::vec3& newValue) noexcept
{
  if (_emissionColor == newValue) return;
  _emissionColor = newValue;
  _materialData.emission = _emissionColor * _emissionFactor;
  emit materialDataChanged(_materialData);
  emit emissionColorChanged(newValue);
}

void MaterialObject::setEmissionFactor(float newValue) noexcept
{
  if (_emissionFactor == newValue) return;
  _emissionFactor = newValue;
  _materialData.emission = _emissionColor * _emissionFactor;
  emit materialDataChanged(_materialData);
  emit emissionFactorChanged(newValue);
}

void MaterialObject::setAlbedoTexture(const QString& newValue) noexcept
{
  if(_albedoTexture == newValue) return;
  _albedoTexture = newValue;
  emit albedoTextureChanged(newValue);
}

void MaterialObject::setUseAlphaFromAlbedoTexture(bool newValue) noexcept
{
  if(_useAlphaFromAlbedoTexture == newValue) return;
  _useAlphaFromAlbedoTexture = newValue;
  emit useAlphaFromAlbedoTextureChanged(newValue);
}

void MaterialObject::setOpaqueTexture(const QString& newValue) noexcept
{
  if (_opaqueTexture == newValue) return;
  _opaqueTexture = newValue;
  emit opaqueTextureChanged(newValue);
}

void MaterialObject::setSpecularTexture(const QString& newValue) noexcept
{
  if (_specularTexture == newValue) return;
  _specularTexture = newValue;
  emit specularTextureChanged(newValue);
}

void MaterialObject::setNormalTexture(const QString& newValue) noexcept
{
  if (_normalTexture == newValue) return;
  _normalTexture = newValue;
  emit normalTextureChanged(newValue);
}

void MaterialObject::setEmissionTexture(const QString& newValue) noexcept
{
  if (_emissionTexture == newValue) return;
  _emissionTexture = newValue;
  emit emissionTextureChanged(newValue);
}

void MaterialObject::setReflectionTexture(const QString& newValue) noexcept
{
  if (_reflectionTexture == newValue) return;
  _reflectionTexture = newValue;
  emit reflectionTextureChanged(newValue);
}
