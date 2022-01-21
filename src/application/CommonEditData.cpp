#include <algorithm>

#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/CommonEditData.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

CommonEditData::CommonEditData(UndoStack& undoStack) :
  _scene(nullptr),
  _undoStack(undoStack)
{
}

std::unique_ptr<Scene> CommonEditData::setScene(std::unique_ptr<Scene> newScene)
{
  _undoStack.clear();
  selectObjects({});

  if(_scene != nullptr)
  {
    disconnect( _scene.get(),
                &Scene::objectUnregistered,
                this,
                &CommonEditData::_onObjectUnregistered);
  }

  std::unique_ptr<Scene> oldScene = std::move(_scene);
  _scene = std::move(newScene);

  if(_scene != nullptr)
  {
    connect(_scene.get(),
            &Scene::objectUnregistered,
            this,
            &CommonEditData::_onObjectUnregistered,
            Qt::DirectConnection);
  }

  emit sceneChanged(_scene.get());
  return oldScene;
}

void CommonEditData::selectObjects(const std::vector<Object*>& objects)
{
  if(_selectedObjects == objects) return;
  for(Object* object : objects)
  {
    if(object == nullptr) Abort("CommonEditData::selectObjects: null object detected.");
  }
  _selectedObjects = objects;
  emit selectedObjectsChanged(objects);
}

void CommonEditData::_onObjectUnregistered(Object& object) noexcept
{
  std::vector<Object*> selection = _selectedObjects;
  selection.erase(std::remove(selection.begin(), selection.end(), &object),
                  selection.end());
  if(selection.size() != _selectedObjects.size()) selectObjects(selection);
}
