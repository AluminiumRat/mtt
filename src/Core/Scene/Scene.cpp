#include <algorithm>
#include <stdexcept>

#include <mtt/Core/Scene/Scene.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

Scene::Scene() :
  _inDestruction(false)
{
}

Scene::~Scene() noexcept
{
  startDestruction();
}

void Scene::registerObject(Object& object)
{
  if(object.scene() == this) return;
  if(object.scene() != nullptr) object.scene()->unregisterObject(object);

  ObjectsMap::iterator iObject = _objectsMap.find(object.id());
  if(iObject != _objectsMap.end())
  {
    if(iObject->second == &object) Abort("Scene::registerObject: the object is already registered in the scene.");
    throw std::runtime_error("Unable to add an object to the scene. Duplicate object id.");
  }

  object._setScene(this);
  _objectsMap.insert(std::make_pair(object.id(), &object));

  for(ObjectsMap::iterator iObject = _objectsMap.begin();
      iObject != _objectsMap.end();
      iObject++)
  {
    if(iObject->second != &object)
    {
      try
      {
        iObject->second->notifyAboutAppearance(object);
        object.notifyAboutAppearance(*iObject->second);
      }
      catch (std::exception& error)
      {
        Log() << "Scene::registerObject: unable to link object: " << error.what();
      }
      catch (...)
      {
        Log() << "Scene::registerObject: unable to link object";
      }
    }
  }

  emit objectRegistered(object);
}

void Scene::unregisterObject(Object& object) noexcept
{
  if(object.scene() != this) Abort("Scene::unregisterObject: the object is not registered in scene.");

  try
  {
    if(!inDestruction()) _objectsMap.erase(object.id());
    object._setScene(nullptr);
  }
  catch(...)
  {
    Abort("Scene::unregisterObject: unable to unregister the object from the scene.");
  }

  for(ObjectsMap::iterator iObject = _objectsMap.begin();
      iObject != _objectsMap.end();
      iObject++)
  {
    iObject->second->notifyAboutDisappearance(object);
    object.notifyAboutDisappearance(*iObject->second);
  }

  emit objectUnregistered(object);
}
