#include <mtt/editorLib/Objects/Object3DGroup.h>

using namespace mtt;

Object3DGroup::Object3DGroup( const QString& name,
                              bool canBeRenamed,
                              const UID& id) :
  SpecialGroup(name, canBeRenamed, id)
{
}
