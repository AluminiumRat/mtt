#include <Objects/CubemapObject.h>

CubemapObject::CubemapObject(const mtt::UID& id) :
  Object(id)
{
}

void CubemapObject::setTextures(const Textures& newValue) noexcept
{
  if(_textures == newValue) return;
  _textures = newValue;
  emit texturesChanged(_textures);
}
