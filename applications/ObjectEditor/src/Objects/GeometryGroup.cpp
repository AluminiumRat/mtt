#include <Objects/GeometryGroup.h>

GeometryGroup::GeometryGroup( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  SpecialGroup(name, canBeRenamed, id)
{
}
