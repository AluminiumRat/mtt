#pragma once

#include <mtt/application/Scene/SpecialGroup.h>
#include <mtt/editorLib/Objects/EnvironmentObject.h>

namespace mtt
{
  class EnvironmentGroup : public SpecialGroup< DisplayedObject,
                                                EnvironmentObject>
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitEnvironmentGroup,
                            visitConstEnvironmentGroup,
                            DisplayedObject)

  public:
    EnvironmentGroup( const QString& name,
                      bool canBeRenamed,
                      const UID& id = UID());
    EnvironmentGroup(const EnvironmentGroup&) = delete;
    EnvironmentGroup& operator = (const EnvironmentGroup&) = delete;
    virtual ~EnvironmentGroup() noexcept = default;

  signals:
    void childAdded(EnvironmentObject& object);
    void childRemoved(EnvironmentObject& object);
  };
}