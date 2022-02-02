#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Mesh/ModificatorProxyTechnique.h>

using namespace mtt;

ModificatorProxyTechnique::~ModificatorProxyTechnique() noexcept
{
  for (TechniqueRecord& record : _subtechniques)
  {
    unregisterSubtechnique(*record.technique);
  }
}

void ModificatorProxyTechnique::registerAreaModificators(
                                                      AreaModificatorSet& set)
{
  CompositeMeshTechnique::registerAreaModificators(set);

  for (TechniqueRecord& record : _subtechniques)
  {
    if (record.set = &set)
    {
      record.registerCounter++;
      return;
    }
  }

  try
  {
    TechniqueRecord newRecord;
    newRecord.set = &set;
    newRecord.registerCounter = 1;
    newRecord.technique = createTechnique(set);

    AbstractMeshTechnique& newTechniqueRef = *newRecord.technique;
    _subtechniques.push_back(std::move(newRecord));
    registerSubtechnique(newTechniqueRef);
  }
  catch (...)
  {
    _subtechniques.pop_back();
    CompositeMeshTechnique::unregisterAreaModificators(set);
  }
}

void ModificatorProxyTechnique::unregisterAreaModificators(
                                              AreaModificatorSet& set) noexcept
{
  CompositeMeshTechnique::unregisterAreaModificators(set);

  for ( Subtechniques::iterator iSubtechnique = _subtechniques.begin();
        iSubtechnique != _subtechniques.end();
        iSubtechnique++)
  {
    if (iSubtechnique->set == &set)
    {
      iSubtechnique->registerCounter--;
      if(iSubtechnique->registerCounter == 0)
      {
        unregisterSubtechnique(*iSubtechnique->technique);
        _subtechniques.erase(iSubtechnique);
      }
      return;
    }
  }
}

void ModificatorProxyTechnique::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  for (TechniqueRecord& record : _subtechniques)
  {
    if (record.set == buildInfo.areaModificators)
    {
      record.technique->addToDrawPlan(buildInfo);
      return;
    }
  }

  Log() << "ModificatorProxyTechnique::addToDrawPlan: no subtechnique was found for the current area modificators set.";
}
