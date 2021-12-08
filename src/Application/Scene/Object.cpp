#include <algorithm>

#include <mtt/Application/Scene/ObjectLink.h>
#include <mtt/Application/Scene/Object.h>
#include <mtt/Application/Scene/Scene.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

Object::Object( const QString& name,
                bool canBeRenamed,
                const UID& id) :
  _id(id),
  _name(name),
  _canBeRenamed(canBeRenamed),
  _scene(nullptr),
  _parent(nullptr)
{
  if (_id == UID(0)) _id = UID(UID::randomValue());
}

void Object::tryRename(const QString& newVale)
{
  if(!_canBeRenamed) return;
  if(_name == newVale) return;
  _name = newVale;
  emit nameChanged(_name);
}

Object::~Object() noexcept
{
  if(_scene != nullptr) _scene->unregisterObject(*this);
  for (Object* object : _subobjects) delete object;
}

void Object::accept(ObjectVisitor& visitor)
{
  visitor.visit(*this);
}

void Object::accept(ObjectVisitor& visitor) const
{
  visitor.visit(*this);
}

void Object::_setScene(Scene* newScene)
{
  Scene* oldScene = scene();

  try
  {
    for (Object* subobject : _subobjects)
    {
      if (oldScene != nullptr) oldScene->unregisterObject(*subobject);
      if (newScene != nullptr) newScene->registerObject(*subobject);
    }
  }
  catch (...)
  {
    Abort("Object::_setScene: unable to register subobject in scene.");
  }

  _scene = newScene;
}

void Object::_setParent(Object* newParent) noexcept
{
  _parent = newParent;
}

void Object::addSubobject(std::unique_ptr<Object> object)
{
  Object* objectPtr = object.get();
  if(objectPtr == nullptr) Abort("Object::addSubobject: the object is null.");
  if (object->parent() != nullptr) Abort("Object::addSubobject: the object already has a parent.");

  if(object->scene() != nullptr && object->scene() != scene())
  {
    object->scene()->unregisterObject(*object);
  }

  if(scene() != nullptr) scene()->registerObject(*object);

  try
  {
    _subobjects.push_back(object.get());
    object.release();
    objectPtr->_setParent(this);

    try
    {
      onSubobjectAdded(*objectPtr);
    }
    catch(...)
    {
      try
      {
        delete objectPtr;
        _subobjects.pop_back();
      }
      catch(...)
      {
        Abort("Object::addSubobject: unable to remove subobject from list.");
      }
      throw;
    }
  }
  catch(...)
  {
    if (scene() != nullptr) scene()->unregisterObject(*object);
    throw;
  }

  emit subobjectAdded(*objectPtr);
}

void Object::onSubobjectAdded(Object& object) noexcept
{
}

std::unique_ptr<Object> Object::removeSubobject(Object& object,
                                                bool removeFromScene) noexcept
{
  if (object.parent() != this) Abort("Object::removeSubobject: the object has a different parent.");

  Subobjects::iterator iSubobject = std::find(_subobjects.begin(),
                                              _subobjects.end(),
                                              &object);
  if(iSubobject == _subobjects.end()) return nullptr;
  try
  {
    _subobjects.erase(iSubobject);
  }
  catch(...)
  {
    Abort("Object::removeSubobject: unable to remove object from container.");
  }
  object._setParent(nullptr);
  onSubobjectRemoved(object);

  emit subobjectRemoved(object);

  if (removeFromScene && scene() != nullptr) scene()->unregisterObject(object);

  return std::unique_ptr<Object>(&object);
}

void Object::onSubobjectRemoved(Object& object) noexcept
{
}

bool Object::subobjectCanBeAdded(const Object& subobject) const noexcept
{
  return false;
}

std::unique_ptr<Object> Object::tryAddSubobject(std::unique_ptr<Object> object)
{
  return object;
}

bool Object::subobjectCanBeRemoved(const Object& subobject) const noexcept
{
  return false;
}

std::unique_ptr<Object> Object::tryRemoveSubobject(
                                                Object& subobject,
                                                bool removeFromScene) noexcept
{
  return nullptr;
}

void Object::notifyAboutAppearance(Object& object)
{
  for(ObjectRefBase* link : _links)
  {
    if(link->referencedId() == object.id()) link->setReferencedPtr(&object);
  }
}

void Object::notifyAboutDisappearance(Object& object) noexcept
{
  for (ObjectRefBase* link : _links)
  {
    if (link->referencedId() == object.id()) link->setReferencedPtr(nullptr);
  }
}

void Object::addLink(ObjectRefBase& link)
{
  if(std::find(_links.begin(), _links.end(), &link) != _links.end())
  {
    Abort("Object::addLink: link is already registered.");
  }
  _links.push_back(&link);

  try
  {
    Object* referenced = nullptr;
    if(_scene != nullptr) referenced = _scene->findObject(link._referencedId);
    link.setReferencedPtr(referenced);
  }
  catch(...)
  {
    _links.pop_back();
    throw;
  }
}

void Object::removeLink(ObjectRefBase& link) noexcept
{
  Links::iterator iLink = std::find(_links.begin(),
                                    _links.end(),
                                    &link);
  if(iLink == _links.end()) Abort("Object::removeLink: link is not registered.");
  link.setReferencedPtr(nullptr);
  _links.erase(iLink);
}

void Object::updateLink(ObjectRefBase& link)
{
  Object* referenced = nullptr;
  if (_scene != nullptr) referenced = _scene->findObject(link._referencedId);
  link.setReferencedPtr(referenced);
}
