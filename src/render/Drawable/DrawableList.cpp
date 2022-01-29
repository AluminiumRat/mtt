#include <algorithm>
#include <stdexcept>

#include <mtt/render/Drawable/DrawableList.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

void DrawableList::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  for(Drawable* child : _childs) child->addToDrawPlan(buildInfo);
}

void DrawableList::addChild(Drawable& newChild)
{
  _childs.push_back(&newChild);
  try
  {
    for(AreaModificatorSet* modificator : _areaModificators)
    {
      newChild.registerAreaModificators(*modificator);
    }
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("DrawableList::addChild: unable to register modificators in child.");
  }
  catch (...)
  {
    Abort("DrawableList::addChild: unable to register modificators in child.");
  }
}

void DrawableList::removeChild(size_t index) noexcept
{
  Drawable* child = _childs[index];
  for (AreaModificatorSet* modificator : _areaModificators)
  {
    child->unregisterAreaModificators(*modificator);
  }
  _childs.erase(_childs.begin() + index);
}

void DrawableList::removeChild(Drawable& removable) noexcept
{
  Childs::iterator iChild = std::find(_childs.begin(),
                                      _childs.end(),
                                      &removable);
  if(iChild == _childs.end()) return;
  removeChild(iChild - _childs.begin());
}

void DrawableList::registerAreaModificators(AreaModificatorSet& set)
{
  Drawable::registerAreaModificators(set);
  _areaModificators.push_back(&set);
  try
  {
    for (Drawable* child : _childs)
    {
      child->registerAreaModificators(set);
    }
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("DrawableList::registerAreaModificators: unable to register modificator in childs.");
  }
  catch (...)
  {
    Abort("DrawableList::registerAreaModificators: unable to register modificator in childs.");
  }
}

void DrawableList::unregisterAreaModificators(AreaModificatorSet& set) noexcept
{
  Drawable::unregisterAreaModificators(set);
  AreaModificators::iterator iModificators = std::find(
                                                    _areaModificators.begin(),
                                                    _areaModificators.end(),
                                                    &set);
  if(iModificators == _areaModificators.end()) return;
  _areaModificators.erase(iModificators);
  for (Drawable* child : _childs)
  {
    child->unregisterAreaModificators(set);
  }
}
