#include <algorithm>

#include <mtt/utilities/Log.h>

#include <Objects/DisplayedObject.h>
#include <Render/AbstractObjectRenderObserver.h>
#include <EditorCommonData.h>

AbstractObjectRenderObserver::AbstractObjectRenderObserver(
                                                DisplayedObject& object,
                                                EditorCommonData& commonData) :
  _object(object),
  _commonData(commonData)
{
  _uidSetter.setUid(object.id());

  connect(&_object,
          &Object3D::completeVisibleChanged,
          this,
          &AbstractObjectRenderObserver::_updateVisible,
          Qt::DirectConnection);
  _updateVisible(_object.completeVisible());

  connect(&_commonData,
          &EditorCommonData::selectedObjectsChanged,
          this,
          &AbstractObjectRenderObserver::_updateSelected,
          Qt::DirectConnection);
  _updateSelected(_commonData.selectedObjects());
}

void AbstractObjectRenderObserver::registerCulledDrawable(
                                                    mtt::DrawableNode& drawable)
{
  if(std::find( _culledDrawables.begin(),
                _culledDrawables.end(),
                &drawable) != _culledDrawables.end())
  {
    mtt::Log() << "AbstractObjectRenderObserver::registerCulledDrawable: drawable is already registered.";
    return;
  }

  _culledDrawables.push_back(&drawable);
  emit culledDrawableRegistered(drawable);
}

void AbstractObjectRenderObserver::unregisterCulledDrawable(
                                          mtt::DrawableNode& drawable) noexcept
{
  CulledDrawables::iterator iDrawable = std::find(_culledDrawables.begin(),
                                                  _culledDrawables.end(),
                                                  &drawable);
  if(iDrawable == _culledDrawables.end())
  {
    mtt::Log() << "AbstractObjectRenderObserver::unregisterCulledDrawable: drawable is not registered.";
    return;
  }

  _culledDrawables.erase(iDrawable);
  emit culledDrawableUnregistered(drawable);
}

void AbstractObjectRenderObserver::registerUnculledDrawable(
                                                        mtt::Drawable& drawable)
{
  if(std::find( _unculledDrawables.begin(),
                _unculledDrawables.end(),
                &drawable) != _unculledDrawables.end())
  {
    mtt::Log() << "AbstractObjectRenderObserver::registerUnculledDrawable: drawable is already registered.";
    return;
  }

  _unculledDrawables.push_back(&drawable);
  emit unculledDrawableRegistered(drawable);
}

void AbstractObjectRenderObserver::unregisterUnculledDrawable(
                                              mtt::Drawable& drawable) noexcept
{
  UnculledDrawables::iterator iDrawable = std::find(_unculledDrawables.begin(),
                                                    _unculledDrawables.end(),
                                                    &drawable);
  if(iDrawable == _unculledDrawables.end())
  {
    mtt::Log() << "AbstractObjectRenderObserver::unregisterUnculledDrawable: drawable is not registered.";
    return;
  }

  _unculledDrawables.erase(iDrawable);
  emit unculledDrawableUnregistered(drawable);
}

void AbstractObjectRenderObserver::registerAreaModificator(
                                              mtt::AreaModificator& modificator)
{
  if(std::find( _areaModificators.begin(),
                _areaModificators.end(),
                &modificator) != _areaModificators.end())
  {
    mtt::Log() << "AbstractObjectRenderObserver::registerAreaModificator: modificator is already registered.";
    return;
  }

  _areaModificators.push_back(&modificator);
  emit areaModificatorRegistered(modificator);
}

void AbstractObjectRenderObserver::unregisterAreaModificator(
                                    mtt::AreaModificator& modificator) noexcept
{
  AreaModificators::iterator iModificator = std::find(_areaModificators.begin(),
                                                      _areaModificators.end(),
                                                      &modificator);
  if(iModificator == _areaModificators.end())
  {
    mtt::Log() << "AbstractObjectRenderObserver::unregisterAreaModificator: modificator is not registered.";
    return;
  }

  _areaModificators.erase(iModificator);
  emit areaModificatorUnregistered(modificator);
}

void AbstractObjectRenderObserver::_updateSelected(
                              const std::vector<mtt::Object*>& objects) noexcept
{
  for(mtt::Object* object : objects)
  {
    if(object == &_object)
    {
      _selectionModificator.setSelected(true);
      return;
    }
  }
  _selectionModificator.setSelected(false);
}

void AbstractObjectRenderObserver::_updateVisible(bool newVisible)
{
  _visibleFilter.setVisible(newVisible);
}
