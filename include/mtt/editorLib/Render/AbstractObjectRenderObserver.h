#pragma once

#include <vector>

#include <QtCore/QObject>

#include <mtt/render/Drawable/Drawable.h>
#include <mtt/render/Drawable/SelectionDrawableModificator.h>
#include <mtt/render/Drawable/UidDrawableModificator.h>
#include <mtt/render/Drawable/VisibleDrawableFilter.h>
#include <mtt/render/SceneGraph/AreaModificator.h>
#include <mtt/render/SceneGraph/DrawableNode.h>
#include <mtt/render/CompositeRenderObject.h>

namespace mtt
{
  class CommonEditData;
  class DisplayedObject;
  class Object;

  class AbstractObjectRenderObserver : public QObject
  {
    Q_OBJECT

  public:
    AbstractObjectRenderObserver( DisplayedObject& object,
                                  CommonEditData& commonData);
    AbstractObjectRenderObserver(const AbstractObjectRenderObserver&) = delete;
    AbstractObjectRenderObserver& operator =
                                (const AbstractObjectRenderObserver&) = delete;
    virtual ~AbstractObjectRenderObserver() noexcept = default;

    inline DisplayedObject& object() const noexcept;
    inline CommonEditData& commonData() const noexcept;

    inline size_t culledDrawablesNumber() const noexcept;
    inline DrawableNode& culledDrawable(size_t index) noexcept;
    inline const DrawableNode& culledDrawable(size_t index) const noexcept;

    inline size_t unculledDrawablesNumber() const noexcept;
    inline Drawable& unculledDrawable(size_t index) noexcept;
    inline const Drawable& unculledDrawable(size_t index) const noexcept;

    inline size_t areaModificatorsNumber() const noexcept;
    inline AreaModificator& areaModificator(size_t index) noexcept;
    inline const AreaModificator& areaModificator(
                                                  size_t index) const noexcept;

    inline size_t compositeObjectsNumber() const noexcept;
    inline CompositeRenderObject& compositeObject(size_t index) noexcept;
    inline const CompositeRenderObject& compositeObject(
                                                  size_t index) const noexcept;

  signals:
    void culledDrawableRegistered(DrawableNode& drawable);
    void culledDrawableUnregistered(DrawableNode& drawable);
    void unculledDrawableRegistered(Drawable& drawable);
    void unculledDrawableUnregistered(Drawable& drawable);
    void areaModificatorRegistered(AreaModificator& modificator);
    void areaModificatorUnregistered(AreaModificator& modificator);
    void compositeObjectRegistered(CompositeRenderObject& theObject);
    void compositeObjectUnregistered(CompositeRenderObject& theObject);

  protected:
    void registerCulledDrawable(DrawableNode& drawable);
    void unregisterCulledDrawable(DrawableNode& drawable) noexcept;
    void registerUnculledDrawable(Drawable& drawable);
    void unregisterUnculledDrawable(Drawable& drawable) noexcept;
    void registerAreaModificator(AreaModificator& modificator);
    void unregisterAreaModificator(AreaModificator& modificator) noexcept;
    void registerCompositeObject(CompositeRenderObject& theObject);
    void unregisterCompositeObject(CompositeRenderObject& theObject) noexcept;

    inline SelectionDrawableModificator& selectionModificator() noexcept;
    inline const SelectionDrawableModificator&
                                          selectionModificator() const noexcept;
    inline VisibleDrawableFilter& visibleFilter() noexcept;
    inline const VisibleDrawableFilter& visibleFilter() const noexcept;
    inline UidDrawableModificator& uidSetter() noexcept;
    inline const UidDrawableModificator& uidSetter() const noexcept;

    virtual void updateSelected(const std::vector<Object*>& objects) noexcept;
    virtual void updateVisible(bool newVisible) noexcept;

  private:
    DisplayedObject& _object;
    CommonEditData& _commonData;

    using CulledDrawables = std::vector<DrawableNode*>;
    CulledDrawables _culledDrawables;

    using UnculledDrawables = std::vector<Drawable*>;
    UnculledDrawables _unculledDrawables;

    using AreaModificators = std::vector<AreaModificator*>;
    AreaModificators _areaModificators;

    using CompositeObjects = std::vector<CompositeRenderObject*>;
    CompositeObjects _compositeObjects;

    SelectionDrawableModificator _selectionModificator;
    VisibleDrawableFilter _visibleFilter;
    UidDrawableModificator _uidSetter;
  };

  inline DisplayedObject& AbstractObjectRenderObserver::object() const noexcept
  {
    return _object;
  }

  inline CommonEditData&
                      AbstractObjectRenderObserver::commonData() const noexcept
  {
    return _commonData;
  }

  inline size_t
            AbstractObjectRenderObserver::culledDrawablesNumber() const noexcept
  {
    return _culledDrawables.size();
  }

  inline DrawableNode&
            AbstractObjectRenderObserver::culledDrawable(size_t index) noexcept
  {
    return *_culledDrawables[index];
  }

  inline const DrawableNode&
      AbstractObjectRenderObserver::culledDrawable(size_t index) const noexcept
  {
    return *_culledDrawables[index];
  }

  inline size_t
          AbstractObjectRenderObserver::unculledDrawablesNumber() const noexcept
  {
    return _unculledDrawables.size();
  }

  inline Drawable&
          AbstractObjectRenderObserver::unculledDrawable(size_t index) noexcept
  {
    return *_unculledDrawables[index];
  }

  inline const Drawable&
    AbstractObjectRenderObserver::unculledDrawable(size_t index) const noexcept
  {
    return *_unculledDrawables[index];
  }

  inline size_t
          AbstractObjectRenderObserver::areaModificatorsNumber() const noexcept
  {
    return _areaModificators.size();
  }

  inline AreaModificator&
            AbstractObjectRenderObserver::areaModificator(size_t index) noexcept
  {
    return *_areaModificators[index];
  }

  inline const AreaModificator&
      AbstractObjectRenderObserver::areaModificator(size_t index) const noexcept
  {
    return *_areaModificators[index];
  }

  inline size_t
          AbstractObjectRenderObserver::compositeObjectsNumber() const noexcept
  {
    return _compositeObjects.size();
  }

  inline CompositeRenderObject&
            AbstractObjectRenderObserver::compositeObject(size_t index) noexcept
  {
    return *_compositeObjects[index];
  }

  inline const CompositeRenderObject&
      AbstractObjectRenderObserver::compositeObject(size_t index) const noexcept
  {
    return *_compositeObjects[index];
  }

  inline SelectionDrawableModificator&
                  AbstractObjectRenderObserver::selectionModificator() noexcept
  {
    return _selectionModificator;
  }

  inline const SelectionDrawableModificator&
            AbstractObjectRenderObserver::selectionModificator() const noexcept
  {
    return _selectionModificator;
  }

  inline VisibleDrawableFilter&
                          AbstractObjectRenderObserver::visibleFilter() noexcept
  {
    return _visibleFilter;
  }

  inline const VisibleDrawableFilter&
                    AbstractObjectRenderObserver::visibleFilter() const noexcept
  {
    return _visibleFilter;
  }

  inline UidDrawableModificator&
                              AbstractObjectRenderObserver::uidSetter() noexcept
  {
    return _uidSetter;
  }

  inline const UidDrawableModificator&
                        AbstractObjectRenderObserver::uidSetter() const noexcept
  {
    return _uidSetter;
  }
}