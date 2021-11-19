#pragma once

#include <memory>
#include <vector>

#include <QtCore/QObject>

#include <mtt/Application/Scene/Scene.h>

namespace mtt
{
  class Object;
  class Scene;
  class UndoStack;

  class CommonEditData : public QObject
  {
    Q_OBJECT

  public:
    explicit CommonEditData(UndoStack& undoStack);
    CommonEditData(const CommonEditData&) = delete;
    CommonEditData& operator = (const CommonEditData&) = delete;
    virtual ~CommonEditData() noexcept = default;

    inline Scene* scene() const noexcept;

    inline UndoStack& undoStack() noexcept;
    inline const UndoStack& undoStack() const noexcept;

    inline const std::vector<Object*>& selectedObjects() const noexcept;
    virtual void selectObjects(const std::vector<Object*>& objects);

  signals:
    void sceneChanged(Scene* newScene);
    void selectedObjectsChanged(const std::vector<Object*>& objects);
  
  protected:
    /// returns old scene
    std::unique_ptr<Scene> setScene(std::unique_ptr<Scene> newScene);

  private:
    void _onObjectUnregistered(Object& object) noexcept;

  private:
    std::unique_ptr<Scene> _scene;
    UndoStack& _undoStack;
    std::vector<Object*> _selectedObjects;
  };

  inline Scene* CommonEditData::scene() const noexcept
  {
    return _scene.get();
  }

  inline UndoStack& CommonEditData::undoStack() noexcept
  {
    return _undoStack;
  }

  inline const UndoStack& CommonEditData::undoStack() const noexcept
  {
    return _undoStack;
  }

  inline const std::vector<Object*>&
                                CommonEditData::selectedObjects() const noexcept
  {
    return _selectedObjects;
  }
}
