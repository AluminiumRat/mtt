#include <algorithm>
#include <stdexcept>

#include <mtt/render/SceneGraph/DrawableNode.h>
#include <mtt/render/SceneGraph/FieldArea.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

FieldArea::~FieldArea() noexcept
{
  _detachModificators();
}

void FieldArea::addDrawable(DrawableNode& node)
{
  _drawables.push_back(&node);
}

void FieldArea::removeDrawable(DrawableNode& node) noexcept
{
  Drawables::iterator iDrawable = std::find(_drawables.begin(),
                                            _drawables.end(),
                                            &node);
  if(iDrawable == _drawables.end()) return;
  _drawables.erase(iDrawable);
}

void FieldArea::_attachModificators()
{
  if (!_modificators.has_value()) return;

  try
  {
    for(DrawableNode* node : _drawables)
    {
      node->registerAreaModificators(_modificators.value());
    }
  }
  catch (...)
  {
    _detachModificators();
    throw;
  }
}

void FieldArea::_detachModificators() noexcept
{
  if(!_modificators.has_value()) return;
  for(DrawableNode* node : _drawables)
  {
    node->unregisterAreaModificators(_modificators.value());
  }
}

void FieldArea::addModificator(AreaModificator& modificator)
{
  std::vector<AreaModificator*> newModificators;
  if(_modificators.has_value()) newModificators = _modificators->modificators();
  newModificators.push_back(&modificator);

  _detachModificators();
  _modificators.emplace(std::move(newModificators));
  _attachModificators();
}

void FieldArea::removeModificator(AreaModificator& modificator) noexcept
{
  if (!_modificators.has_value()) return;

  using Modificators = std::vector<AreaModificator*>;
  Modificators newModificators = _modificators->modificators();
  Modificators::iterator iModificator = std::find(newModificators.begin(),
                                                  newModificators.end(),
                                                  &modificator);
  if(iModificator == newModificators.end()) return;
  newModificators.erase(iModificator);

  _detachModificators();
  try
  {
    _modificators.emplace(std::move(newModificators));
    _attachModificators();
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("FieldArea::removeModificator: unable to remove area modificator.");
  }
  catch (...)
  {
    Abort("FieldArea::removeModificator: unable to remove area modificator.");
  }
}
