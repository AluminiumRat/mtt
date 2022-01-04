#include <Objects/EnvironmentGroup.h>

EnvironmentGroup::EnvironmentGroup( const QString& name,
                                    bool canBeRenamed,
                                    const mtt::UID& theId) :
  SpecialGroup(name, canBeRenamed, theId)
{
}
