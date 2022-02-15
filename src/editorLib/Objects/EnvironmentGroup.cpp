#include <mtt/editorLib/Objects/EnvironmentGroup.h>

using namespace mtt;

EnvironmentGroup::EnvironmentGroup( const QString& name,
                                    bool canBeRenamed,
                                    const UID& theId) :
  SpecialGroup(name, canBeRenamed, theId)
{
}
