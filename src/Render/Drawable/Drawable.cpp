#include <algorithm>
#include <stdexcept>

#include <mtt/Render/Drawable/Drawable.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

void Drawable::registedModificator(Modificator& modificator)
{
  if(std::find( _modificators.begin(),
                _modificators.end(),
                &modificator) != _modificators.end()) return;
  _modificators.push_back(&modificator);
}

void Drawable::unregistedModificator(Modificator& modificator) noexcept
{
  Modificators::iterator iModificator = std::find(_modificators.begin(),
                                                  _modificators.end(),
                                                  &modificator);
  if(iModificator == _modificators.end()) return;
  try
  {
    _modificators.erase(iModificator);
  }
  catch(std::exception& error)
  {
    Log() << error.what();
    Abort("Drawable::unregistedModificator: unable to unregister modificator");
  }
  catch(...)
  {
    Abort("Drawable::unregistedModificator: unknown error");
  }
}
