#include <mtt/editorLib/Objects/SkeletonGroup.h>

using namespace mtt;

SkeletonGroup::SkeletonGroup( const QString& name,
                              bool canBeRenamed,
                              const UID& id) :
  SpecialGroup(name, canBeRenamed, id)
{
}
