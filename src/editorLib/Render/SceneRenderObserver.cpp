#include <stdexcept>

#include <mtt/application/Scene/Scene.h>
#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/Render/RenderObserverFactory.h>
#include <mtt/editorLib/Render/SceneRenderObserver.h>
#include <mtt/render/RenderScene.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

SceneRenderObserver::SceneRenderObserver(
                                      Scene& scene,
                                      RenderScene& renderScene,
                                      RenderObserverFactory& observerFactory) :
  _scene(scene),
  _renderScene(renderScene),
  _observerFactory(observerFactory)
{
  connect(&_scene,
          &Scene::objectRegistered,
          this,
          &SceneRenderObserver::_addObject,
          Qt::DirectConnection);

  connect(&_scene,
          &Scene::objectUnregistered,
          this,
          &SceneRenderObserver::_removeObject,
          Qt::DirectConnection);

  auto addDelegate =  [&](Object& theObject)
                      {
                        _addObject(theObject);
                      };
  _scene.processObjects(addDelegate);
}

SceneRenderObserver::~SceneRenderObserver() noexcept
{
  while (!_objectObservers.empty())
  {
    _removeObject(_objectObservers.front()->object());
  }
}

void SceneRenderObserver::_addObject(Object& object) noexcept
{
  try
  {
    std::unique_ptr<AbstractObjectRenderObserver> observer =
                          _observerFactory.buildObserver(object, _renderScene);
    AbstractObjectRenderObserver* observerPtr = observer.get();

    if (observerPtr != nullptr)
    {
      _objectObservers.push_back(std::move(observer));

      connect(observerPtr,
              &AbstractObjectRenderObserver::culledDrawableRegistered,
              this,
              &SceneRenderObserver::_addCulled,
              Qt::DirectConnection);

      connect(observerPtr,
              &AbstractObjectRenderObserver::culledDrawableUnregistered,
              this,
              &SceneRenderObserver::_removeCulled,
              Qt::DirectConnection);

      for(size_t drawableIndex = 0;
          drawableIndex != observerPtr->culledDrawablesNumber();
          drawableIndex++)
      {
        _addCulled(observerPtr->culledDrawable(drawableIndex));
      }

      connect(observerPtr,
              &AbstractObjectRenderObserver::unculledDrawableRegistered,
              this,
              &SceneRenderObserver::_addUnculled,
              Qt::DirectConnection);

      connect(observerPtr,
              &AbstractObjectRenderObserver::unculledDrawableUnregistered,
              this,
              &SceneRenderObserver::_removeUnculled,
              Qt::DirectConnection);

      for(size_t drawableIndex = 0;
          drawableIndex != observerPtr->unculledDrawablesNumber();
          drawableIndex++)
      {
        _addUnculled(observerPtr->unculledDrawable(drawableIndex));
      }

      connect(observerPtr,
              &AbstractObjectRenderObserver::areaModificatorRegistered,
              this,
              &SceneRenderObserver::_addModificator,
              Qt::DirectConnection);

      connect(observerPtr,
              &AbstractObjectRenderObserver::areaModificatorUnregistered,
              this,
              &SceneRenderObserver::_removeModificator,
              Qt::DirectConnection);

      for(size_t modificatorIndex = 0;
          modificatorIndex != observerPtr->areaModificatorsNumber();
          modificatorIndex++)
      {
        _addModificator(observerPtr->areaModificator(modificatorIndex));
      }

      connect(observerPtr,
              &AbstractObjectRenderObserver::compositeObjectRegistered,
              this,
              &SceneRenderObserver::_addCompositeObject,
              Qt::DirectConnection);

      connect(observerPtr,
              &AbstractObjectRenderObserver::compositeObjectUnregistered,
              this,
              &SceneRenderObserver::_removeCompositeObject,
              Qt::DirectConnection);

      for(size_t objectIndex = 0;
          objectIndex != observerPtr->compositeObjectsNumber();
          objectIndex++)
      {
        _addCompositeObject(observerPtr->compositeObject(objectIndex));
      }
    }
  }
  catch(std::exception& error)
  {
    Log() << "SceneRenderObserver::_addObject: " << error.what();
  }
  catch(...)
  {
    Log() << "SceneRenderObserver::_addObject: unknown error.";
  }
}

void SceneRenderObserver::_removeObject(Object& object) noexcept
{
  for(ObjectObservers::iterator iObserver = _objectObservers.begin();
      iObserver != _objectObservers.end();
      iObserver++)
  {
    AbstractObjectRenderObserver& observer = *iObserver->get();
    if(&observer.object() == &object)
    {
      for(size_t drawableIndex = 0;
          drawableIndex != observer.culledDrawablesNumber();
          drawableIndex++)
      {
        _removeCulled(observer.culledDrawable(drawableIndex));
      }

      for(size_t drawableIndex = 0;
          drawableIndex != observer.unculledDrawablesNumber();
          drawableIndex++)
      {
        _removeUnculled(observer.unculledDrawable(drawableIndex));
      }

      for(size_t modificatorIndex = 0;
          modificatorIndex != observer.areaModificatorsNumber();
          modificatorIndex++)
      {
        _removeModificator(observer.areaModificator(modificatorIndex));
      }

      for(size_t objectIndex = 0;
          objectIndex != observer.compositeObjectsNumber();
          objectIndex++)
      {
        _removeCompositeObject(observer.compositeObject(objectIndex));
      }

      _objectObservers.erase(iObserver);
      break;
    }
  }
}

void SceneRenderObserver::_addCulled(DrawableNode& drawable) noexcept
{
  try
  {
    _renderScene.addCulledDrawable(drawable);
  }
  catch(const std::exception& error)
  {
    Log() << "SceneRenderObserver::_addCulled: " << error.what();
  }
  catch(...)
  {
    Log() << "SceneRenderObserver::_addCulled: unknown error";
  }
}

void SceneRenderObserver::_removeCulled(DrawableNode& drawable) noexcept
{
  _renderScene.removeCulledDrawable(drawable);
}

void SceneRenderObserver::_addUnculled(Drawable& drawable) noexcept
{
  try
  {
    _renderScene.addUnculledDrawable(drawable);
  }
  catch(const std::exception& error)
  {
    Log() << "SceneRenderObserver::_addUnculled: " << error.what();
  }
  catch(...)
  {
    Log() << "SceneRenderObserver::_addUnculled: unknown error";
  }
}

void SceneRenderObserver::_removeUnculled(Drawable& drawable) noexcept
{
  _renderScene.removeUnculledDrawable(drawable);
}

void SceneRenderObserver::_addModificator(
                                        AreaModificator& modificator) noexcept
{
  try
  {
    _renderScene.addAreaModificator(modificator);
  }
  catch(const std::exception& error)
  {
    Log() << "SceneRenderObserver::_addModificator: " << error.what();
  }
  catch(...)
  {
    Log() << "SceneRenderObserver::_addModificator: unknown error";
  }
}

void SceneRenderObserver::_removeModificator(
                                        AreaModificator& modificator) noexcept
{
  _renderScene.removeAreaModificator(modificator);
}

void SceneRenderObserver::_addCompositeObject(
                                    CompositeRenderObject& theObject) noexcept
{
  try
  {
    _renderScene.addCompositeObject(theObject);
  }
  catch(const std::exception& error)
  {
    Log() << "SceneRenderObserver::_addCompositeObject: " << error.what();
  }
  catch(...)
  {
    Log() << "SceneRenderObserver::_addCompositeObject: unknown error";
  }
}

void SceneRenderObserver::_removeCompositeObject(
                                    CompositeRenderObject& theObject) noexcept
{
  _renderScene.removeCompositeObject(theObject);
}
