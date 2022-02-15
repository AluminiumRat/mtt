#include <mtt/editorLib/Objects/AnimationGroup.h>

using namespace mtt;

AnimationGroup::AnimationGroup( const QString& name,
                                bool canBeRenamed,
                                const UID& id) :
  SpecialGroup(name, canBeRenamed, id)
{
}
