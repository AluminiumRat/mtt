#include <Objects/ModificatorGroup.h>

ModificatorGroup::ModificatorGroup( const QString& name,
                                    bool canBeRenamed,
                                    const mtt::UID& id) :
  SpecialGroup(name, canBeRenamed, id)
{
}
