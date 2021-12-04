#include <Objects/AnimationGroup.h>

AnimationGroup::AnimationGroup( const QString& name,
                                bool canBeRenamed,
                                const mtt::UID& id) :
  SpecialGroup(name, canBeRenamed, id)
{
}
