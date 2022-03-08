#include <Objects/ModificatorObject.h>

ModificatorObject::ModificatorObject( const QString& name,
                                      bool canBeRenamed,
                                      const mtt::UID& id) :
  ScalableObject(name, canBeRenamed, id)
{
}
