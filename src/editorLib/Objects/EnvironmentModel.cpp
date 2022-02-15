#include <mtt/editorLib/Objects/EnvironmentModel.h>

using namespace mtt;

EnvironmentModel::EnvironmentModel( const QString& name,
                                    bool canBeRenmed,
                                    const UID id) :
                                    EnvironmentObject(name, canBeRenmed, id)
{
}

void EnvironmentModel::setFilename(const QString& newValue) noexcept
{
  if(_filename == newValue) return;
  _filename = newValue;
  emit filenameChanged(newValue);
}
