#include <algorithm>
#include <stdexcept>

#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

SimpleDrawableNode::SimpleDrawableNode() noexcept :
  _drawable(nullptr)
{
}

void SimpleDrawableNode::setDrawable( Drawable* newDrawable,
                                      const Sphere& localBoundSphere) noexcept
{
  if(newDrawable != _drawable)
  {
    if (_drawable != nullptr)
    {
      for (AreaModificatorSet* modificator : _areaModificators)
      {
        _drawable->unregisterAreaModificators(*modificator);
      }
    }

    _drawable = newDrawable;

    if (_drawable != nullptr)
    {
      try
      {
        for (AreaModificatorSet* modificator : _areaModificators)
        {
          _drawable->registerAreaModificators(*modificator);
        }
      }
      catch (std::exception& error)
      {
        Log() << error.what();
        Abort("SimpleDrawableNode::setDrawable: unable to register area modificators in child drawable.");
      }
      catch (...)
      {
        Abort("SimpleDrawableNode::setDrawable: unable to register area modificators in child drawable.");
      }
    }
  }

  setLocalBoundSphere(localBoundSphere);
}

void SimpleDrawableNode::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  if(_drawable != nullptr) _drawable->addToDrawPlan(buildInfo);
}

void SimpleDrawableNode::registerAreaModificators(AreaModificatorSet& set)
{
  DrawableNode::registerAreaModificators(set);
  _areaModificators.push_back(&set);
  if (_drawable != nullptr)
  {
    try
    {
      _drawable->registerAreaModificators(set);
    }
    catch (...)
    {
      _areaModificators.pop_back();
      throw;
    }
  }
}

void SimpleDrawableNode::unregisterAreaModificators(
                                              AreaModificatorSet& set) noexcept
{
  DrawableNode::unregisterAreaModificators(set);
  AreaModificators::iterator iModificator = std::find(_areaModificators.begin(),
                                                      _areaModificators.end(),
                                                      &set);
  if(iModificator == _areaModificators.end()) return;
  _areaModificators.erase(iModificator);
  if(_drawable != nullptr) _drawable->unregisterAreaModificators(set);
}
