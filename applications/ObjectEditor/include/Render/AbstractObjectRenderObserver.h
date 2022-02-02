#pragma once

#include <vector>

#include <QtCore/QObject>

#include <mtt/render/Drawable/Drawable.h>
#include <mtt/render/Drawable/SelectionDrawableModificator.h>
#include <mtt/render/Drawable/UidDrawableModificator.h>
#include <mtt/render/Drawable/VisibleDrawableFilter.h>
#include <mtt/render/SceneGraph/AreaModificator.h>
#include <mtt/render/SceneGraph/DrawableNode.h>

namespace mtt
{
  class Object;
}

class DisplayedObject;
class EditorCommonData;

class AbstractObjectRenderObserver : public QObject
{
  Q_OBJECT

public:
  AbstractObjectRenderObserver( DisplayedObject& object,
                                EditorCommonData& commonData);
  AbstractObjectRenderObserver(const AbstractObjectRenderObserver&) = delete;
  AbstractObjectRenderObserver& operator =
                                (const AbstractObjectRenderObserver&) = delete;
  virtual ~AbstractObjectRenderObserver() noexcept = default;

  inline DisplayedObject& object() const noexcept;
  inline EditorCommonData& commonData() const noexcept;

  inline size_t culledDrawablesNumber() const noexcept;
  inline mtt::DrawableNode& culledDrawable(size_t index) noexcept;
  inline const mtt::DrawableNode& culledDrawable(size_t index) const noexcept;

  inline size_t unculledDrawablesNumber() const noexcept;
  inline mtt::Drawable& unculledDrawable(size_t index) noexcept;
  inline const mtt::Drawable& unculledDrawable(size_t index) const noexcept;

  inline size_t areaModificatorsNumber() const noexcept;
  inline mtt::AreaModificator& areaModificator(size_t index) noexcept;
  inline const mtt::AreaModificator& areaModificator(
                                                  size_t index) const noexcept;

signals:
  void culledDrawableRegistered(mtt::DrawableNode& drawable);
  void culledDrawableUnregistered(mtt::DrawableNode& drawable);
  void unculledDrawableRegistered(mtt::Drawable& drawable);
  void unculledDrawableUnregistered(mtt::Drawable& drawable);
  void areaModificatorRegistered(mtt::AreaModificator& modificator);
  void areaModificatorUnregistered(mtt::AreaModificator& modificator);

protected:
  void registerCulledDrawable(mtt::DrawableNode& drawable);
  void unregisterCulledDrawable(mtt::DrawableNode& drawable) noexcept;
  void registerUnculledDrawable(mtt::Drawable& drawable);
  void unregisterUnculledDrawable(mtt::Drawable& drawable) noexcept;
  void registerAreaModificator(mtt::AreaModificator& modificator);
  void unregisterAreaModificator(mtt::AreaModificator& modificator) noexcept;

  inline mtt::SelectionDrawableModificator& selectionModificator() noexcept;
  inline const mtt::SelectionDrawableModificator&
                                          selectionModificator() const noexcept;
  inline mtt::VisibleDrawableFilter& visibleFilter() noexcept;
  inline const mtt::VisibleDrawableFilter& visibleFilter() const noexcept;
  inline mtt::UidDrawableModificator& uidSetter() noexcept;
  inline const mtt::UidDrawableModificator& uidSetter() const noexcept;

  virtual void updateSelected(
                            const std::vector<mtt::Object*>& objects) noexcept;
  virtual void updateVisible(bool newVisible) noexcept;

private:
  DisplayedObject& _object;
  EditorCommonData& _commonData;

  using CulledDrawables = std::vector<mtt::DrawableNode*>;
  CulledDrawables _culledDrawables;

  using UnculledDrawables = std::vector<mtt::Drawable*>;
  UnculledDrawables _unculledDrawables;

  using AreaModificators = std::vector<mtt::AreaModificator*>;
  AreaModificators _areaModificators;

  mtt::SelectionDrawableModificator _selectionModificator;
  mtt::VisibleDrawableFilter _visibleFilter;
  mtt::UidDrawableModificator _uidSetter;
};

inline DisplayedObject& AbstractObjectRenderObserver::object() const noexcept
{
  return _object;
}

inline EditorCommonData&
                      AbstractObjectRenderObserver::commonData() const noexcept
{
  return _commonData;
}

inline size_t
            AbstractObjectRenderObserver::culledDrawablesNumber() const noexcept
{
  return _culledDrawables.size();
}

inline mtt::DrawableNode&
            AbstractObjectRenderObserver::culledDrawable(size_t index) noexcept
{
  return *_culledDrawables[index];
}

inline const mtt::DrawableNode&
      AbstractObjectRenderObserver::culledDrawable(size_t index) const noexcept
{
  return *_culledDrawables[index];
}

inline size_t
          AbstractObjectRenderObserver::unculledDrawablesNumber() const noexcept
{
  return _unculledDrawables.size();
}

inline mtt::Drawable&
          AbstractObjectRenderObserver::unculledDrawable(size_t index) noexcept
{
  return *_unculledDrawables[index];
}

inline const mtt::Drawable&
    AbstractObjectRenderObserver::unculledDrawable(size_t index) const noexcept
{
  return *_unculledDrawables[index];
}

inline size_t
          AbstractObjectRenderObserver::areaModificatorsNumber() const noexcept
{
  return _areaModificators.size();
}

inline mtt::AreaModificator&
          AbstractObjectRenderObserver::areaModificator(size_t index) noexcept
{
  return *_areaModificators[index];
}

inline const mtt::AreaModificator&
      AbstractObjectRenderObserver::areaModificator(size_t index) const noexcept
{
  return *_areaModificators[index];
}

inline mtt::SelectionDrawableModificator&
                  AbstractObjectRenderObserver::selectionModificator() noexcept
{
  return _selectionModificator;
}

inline const mtt::SelectionDrawableModificator&
            AbstractObjectRenderObserver::selectionModificator() const noexcept
{
  return _selectionModificator;
}

inline mtt::VisibleDrawableFilter&
                          AbstractObjectRenderObserver::visibleFilter() noexcept
{
  return _visibleFilter;
}

inline const mtt::VisibleDrawableFilter&
                    AbstractObjectRenderObserver::visibleFilter() const noexcept
{
  return _visibleFilter;
}

inline mtt::UidDrawableModificator&
                              AbstractObjectRenderObserver::uidSetter() noexcept
{
  return _uidSetter;
}

inline const mtt::UidDrawableModificator&
                        AbstractObjectRenderObserver::uidSetter() const noexcept
{
  return _uidSetter;
}
