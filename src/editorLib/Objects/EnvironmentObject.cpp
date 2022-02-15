#include <mtt/editorLib/Objects/EnvironmentObject.h>

using namespace mtt;

EnvironmentObject::EnvironmentObject( const QString& name,
                                      bool canBeRenamed,
                                      const UID theId) :
  MovableObject(name, canBeRenamed, theId)
{
}