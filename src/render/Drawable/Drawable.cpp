#include <algorithm>
#include <stdexcept>

#include <mtt/render/Drawable/Drawable.h>
#include <mtt/render/Drawable/DrawableModificator.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

void Drawable::addModificator(DrawableModificator& modificator)
{
  if(std::find( _modificators.begin(),
                _modificators.end(),
                &modificator) != _modificators.end()) return;
  _modificators.push_back(&modificator);
}

void Drawable::removeModificator(DrawableModificator& modificator) noexcept
{
  Modificators::iterator iModificator = std::find(_modificators.begin(),
                                                  _modificators.end(),
                                                  &modificator);
  if(iModificator == _modificators.end()) return;
  _modificators.erase(iModificator);
}

void Drawable::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  if(_modificators.empty()) buildDrawActions(buildInfo);
  else
  {
    _modificators[0]->draw( buildInfo,
                            &_modificators[0] + 1,
                            _modificators.size() - 1,
                            *this);
  }
}

void Drawable::registerAreaModificators(AreaModificatorSet& set)
{
}

void Drawable::unregisterAreaModificators(AreaModificatorSet& set) noexcept
{
}
