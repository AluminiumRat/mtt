#include <mtt/editorLib/Objects/CubemapObject.h>

using namespace mtt;

CubemapObject::CubemapObject( const QString& name,
                              bool canBeRenamed,
                              const UID& id) :
  Object(name, canBeRenamed, id)
{
}

void CubemapObject::setTextures(const Textures& newValue) noexcept
{
  if(_textures == newValue) return;
  _textures = newValue;
  emit texturesChanged(_textures);
}
