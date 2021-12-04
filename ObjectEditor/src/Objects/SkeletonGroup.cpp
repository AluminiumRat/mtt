#include <Objects/SkeletonGroup.h>

SkeletonGroup::SkeletonGroup( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  SpecialGroup(name, canBeRenamed, id)
{
}
