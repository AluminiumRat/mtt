#include <Objects/EmitterObject.h>

EmitterObject::EmitterObject( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  ModificatorObject(name, canBeRenamed, id)
{
}
