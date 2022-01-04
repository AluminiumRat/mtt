#include <Objects/EnvironmentObject.h>

EnvironmentObject::EnvironmentObject( const QString& name,
                                      bool canBeRenamed,
                                      const mtt::UID theId) :
  MovableObject(name, canBeRenamed, theId)
{
}