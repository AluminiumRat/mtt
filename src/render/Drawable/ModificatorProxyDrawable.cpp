#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Drawable/ModificatorProxyDrawable.h>

using namespace mtt;

void ModificatorProxyDrawable::registerAreaModificators(AreaModificatorSet& set)
{
  DrawableList::registerAreaModificators(set);

  for (SubdrawableRecord& record : _subdrawables)
  {
    if (record.set = &set)
    {
      record.registerCounter++;
      return;
    }
  }

  try
  {
    SubdrawableRecord newRecord;
    newRecord.set = &set;
    newRecord.registerCounter = 1;
    newRecord.subdrawable = createSubdrawable(set);

    Drawable& newDrawableRef = *newRecord.subdrawable;
    _subdrawables.push_back(std::move(newRecord));
    addChild(newDrawableRef);
  }
  catch (...)
  {
    if(!_subdrawables.empty() && _subdrawables.back().set == &set)
    {
      _subdrawables.pop_back();
    }
    DrawableList::unregisterAreaModificators(set);
    throw;
  }
}

void ModificatorProxyDrawable::unregisterAreaModificators(
                                              AreaModificatorSet& set) noexcept
{
  DrawableList::unregisterAreaModificators(set);

  for (Subdrawables::iterator iSubdrawable = _subdrawables.begin();
        iSubdrawable != _subdrawables.end();
        iSubdrawable++)
  {
    if (iSubdrawable->set == &set)
    {
      iSubdrawable->registerCounter--;
      if(iSubdrawable->registerCounter == 0)
      {
        removeChild(*iSubdrawable->subdrawable);
        _subdrawables.erase(iSubdrawable);
      }
      return;
    }
  }
}

void ModificatorProxyDrawable::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  for (SubdrawableRecord& record : _subdrawables)
  {
    if (record.set == buildInfo.areaModificators)
    {
      record.subdrawable->addToDrawPlan(buildInfo);
      return;
    }
  }

  Log() << "ModificatorProxyDrawable::addToDrawPlan: no subdrawable was found for the current area modificators set.";
}
