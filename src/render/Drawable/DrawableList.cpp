#include <mtt/render/Drawable/DrawableList.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

void DrawableList::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  for(Drawable* child : _childs) child->addToDrawPlan(buildInfo);
}

void DrawableList::removeChild(size_t index) noexcept
{
  try
  {
    _childs.erase(_childs.begin() + index);
  }
  catch(...)
  {
    Abort("DrawableList::removeChild: unable to remove the child.");
  }
}

void DrawableList::removeChild(Drawable& removable) noexcept
{
  try
  {
    Childs::iterator iChild = _childs.begin();
    while(iChild != _childs.end())
    {
      if(*iChild == &removable) iChild = _childs.erase(iChild);
      else iChild++;
    }
  }
  catch (...)
  {
    Abort("DrawableList::removeChild: unable to remove childs.");
  }
}
