#include <Objects/HierarhicalObject.h>

HierarhicalObject::HierarhicalObject( const QString& name,
                                      bool canBeRenamed,
                                      const mtt::UID& id) :
  ScalableObject(name, canBeRenamed, id)
{
}
