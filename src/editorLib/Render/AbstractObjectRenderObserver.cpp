#include <algorithm>

#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/Objects/DisplayedObject.h>
#include <mtt/editorLib/Render/AbstractObjectRenderObserver.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

AbstractObjectRenderObserver::AbstractObjectRenderObserver(
                                                  DisplayedObject& object,
                                                  CommonEditData& commonData) :
  _object(object),
  _commonData(commonData)
{
  _uidSetter.setUid(object.id());

  connect(&_object,
          &DisplayedObject::completeVisibleChanged,
          this,
          &AbstractObjectRenderObserver::updateVisible,
          Qt::DirectConnection);
  updateVisible(_object.completeVisible());

  connect(&_commonData,
          &CommonEditData::selectedObjectsChanged,
          this,
          &AbstractObjectRenderObserver::updateSelected,
          Qt::DirectConnection);
  updateSelected(_commonData.selectedObjects());
}

void AbstractObjectRenderObserver::registerCulledDrawable(
                                                        DrawableNode& drawable)
{
  if(std::find( _culledDrawables.begin(),
                _culledDrawables.end(),
                &drawable) != _culledDrawables.end())
  {
    Log() << "AbstractObjectRenderObserver::registerCulledDrawable: drawable is already registered.";
    return;
  }

  _culledDrawables.push_back(&drawable);
  emit culledDrawableRegistered(drawable);
}

void AbstractObjectRenderObserver::unregisterCulledDrawable(
                                              DrawableNode& drawable) noexcept
{
  CulledDrawables::iterator iDrawable = std::find(_culledDrawables.begin(),
                                                  _culledDrawables.end(),
                                                  &drawable);
  if(iDrawable == _culledDrawables.end())
  {
    Log() << "AbstractObjectRenderObserver::unregisterCulledDrawable: drawable is not registered.";
    return;
  }

  _culledDrawables.erase(iDrawable);
  emit culledDrawableUnregistered(drawable);
}

void AbstractObjectRenderObserver::registerUnculledDrawable(Drawable& drawable)
{
  if(std::find( _unculledDrawables.begin(),
                _unculledDrawables.end(),
                &drawable) != _unculledDrawables.end())
  {
    Log() << "AbstractObjectRenderObserver::registerUnculledDrawable: drawable is already registered.";
    return;
  }

  _unculledDrawables.push_back(&drawable);
  emit unculledDrawableRegistered(drawable);
}

void AbstractObjectRenderObserver::unregisterUnculledDrawable(
                                                  Drawable& drawable) noexcept
{
  UnculledDrawables::iterator iDrawable = std::find(_unculledDrawables.begin(),
                                                    _unculledDrawables.end(),
                                                    &drawable);
  if(iDrawable == _unculledDrawables.end())
  {
    Log() << "AbstractObjectRenderObserver::unregisterUnculledDrawable: drawable is not registered.";
    return;
  }

  _unculledDrawables.erase(iDrawable);
  emit unculledDrawableUnregistered(drawable);
}

void AbstractObjectRenderObserver::registerAreaModificator(
                                                  AreaModificator& modificator)
{
  if(std::find( _areaModificators.begin(),
                _areaModificators.end(),
                &modificator) != _areaModificators.end())
  {
    Log() << "AbstractObjectRenderObserver::registerAreaModificator: modificator is already registered.";
    return;
  }

  _areaModificators.push_back(&modificator);
  emit areaModificatorRegistered(modificator);
}

void AbstractObjectRenderObserver::unregisterAreaModificator(
                                        AreaModificator& modificator) noexcept
{
  AreaModificators::iterator iModificator = std::find(_areaModificators.begin(),
                                                      _areaModificators.end(),
                                                      &modificator);
  if(iModificator == _areaModificators.end())
  {
    Log() << "AbstractObjectRenderObserver::unregisterAreaModificator: modificator is not registered.";
    return;
  }

  _areaModificators.erase(iModificator);
  emit areaModificatorUnregistered(modificator);
}

void AbstractObjectRenderObserver::registerCompositeObject(
                                              CompositeRenderObject& theObject)
{
  if(std::find( _compositeObjects.begin(),
                _compositeObjects.end(),
                &theObject) != _compositeObjects.end())
  {
    Log() << "AbstractObjectRenderObserver::registerCompositeObject: object is already registered.";
    return;
  }

  _compositeObjects.push_back(&theObject);
  emit compositeObjectRegistered(theObject);
}

void AbstractObjectRenderObserver::unregisterCompositeObject(
                                    CompositeRenderObject& theObject) noexcept
{
  CompositeObjects::iterator iObject = std::find( _compositeObjects.begin(),
                                                  _compositeObjects.end(),
                                                  &theObject);
  if(iObject == _compositeObjects.end())
  {
    Log() << "AbstractObjectRenderObserver::unregisterCompositeObject: object is not registered.";
    return;
  }

  _compositeObjects.erase(iObject);
  emit compositeObjectUnregistered(theObject);
}

void AbstractObjectRenderObserver::updateSelected(
                                  const std::vector<Object*>& objects) noexcept
{
  for(Object* object : objects)
  {
    if(object == &_object)
    {
      _selectionModificator.setSelected(true);
      return;
    }
  }
  _selectionModificator.setSelected(false);
}

void AbstractObjectRenderObserver::updateVisible(bool newVisible) noexcept
{
  _visibleFilter.setVisible(newVisible);
}
