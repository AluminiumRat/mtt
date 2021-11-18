#pragma once

#include <map>

#include <QtCore/QObject>

#include <mtt/Core/Scene/Object.h>

namespace mtt
{
  class Scene : public QObject
  {
    Q_OBJECT
  
  public:
    Scene();
    Scene(const Scene&) = delete;
    Scene& operator = (const Scene&) = delete;
    virtual ~Scene() noexcept;

    virtual void registerObject(Object& object);
    virtual void unregisterObject(Object& object) noexcept;

    inline size_t objectsNumber() const noexcept;

    /// Delegate should have signature void(Object&) or void(Object&)
    template <typename Delegate>
    inline void processObjects(Delegate& theDelegate);

    /// Delegate should have signature void(const Object&)
    template <typename Delegate>
    inline void processObjects(Delegate& theDelegate) const;

    inline Object* findObject(const UID& id) noexcept;
    inline const Object* findObject(const UID& id) const noexcept;

    /// Destruction mode is bypassing the bureaucracy in destroying the scene.
    /// Call the startDestruction method in a destructor of derived classes.
    inline bool inDestruction() const noexcept;

  signals:
    void objectRegistered(Object& newObject);
    void objectUnregistered(Object& removedObject);

  protected:
    /// Destruction mode is bypassing the bureaucracy in destroying the scene.
    /// Call the startDestruction method in a destructor of derived classes.
    inline void startDestruction() noexcept;

  private:
    using ObjectsMap = std::map<UID, Object*>;
    ObjectsMap _objectsMap;

    bool _inDestruction;
  };

  inline Object* Scene::findObject(const UID& id) noexcept
  {
    ObjectsMap::const_iterator iObject = _objectsMap.find(id);
    if(iObject == _objectsMap.end()) return nullptr;
    return iObject->second;
  }

  template <typename Delegate>
  inline void Scene::processObjects(Delegate& theDelegate)
  {
    for(ObjectsMap::iterator iObject = _objectsMap.begin();
        iObject != _objectsMap.end();
        iObject++)
    {
      theDelegate(*iObject->second);
    }
  }

  template <typename Delegate>
  inline void Scene::processObjects(Delegate& theDelegate) const
  {
    for(ObjectsMap::const_iterator iObject = _objectsMap.begin();
        iObject != _objectsMap.end();
        iObject++)
    {
      theDelegate(*iObject->second);
    }
  }

  inline size_t Scene::objectsNumber() const noexcept
  {
    return _objectsMap.size();
  }

  inline const Object* Scene::findObject(const UID& id) const noexcept
  {
    ObjectsMap::const_iterator iObject = _objectsMap.find(id);
    if (iObject == _objectsMap.end()) return nullptr;
    return iObject->second;
  }

  inline bool Scene::inDestruction() const noexcept
  {
    return _inDestruction;
  }

  inline void Scene::startDestruction() noexcept
  {
    _inDestruction = true;
  }
}
