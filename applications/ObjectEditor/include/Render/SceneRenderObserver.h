#pragma once

#include <memory>
#include <vector>

#include <QtCore/QObject>

#include <Render/AbstractObjectRenderObserver.h>

namespace mtt
{
  class Object;
  class RenderScene;
}

class EditorCommonData;
class EditorScene;

class SceneRenderObserver : public QObject
{
  Q_OBJECT

public:
  explicit SceneRenderObserver(EditorCommonData& commonData);
  SceneRenderObserver(const SceneRenderObserver&) = delete;
  SceneRenderObserver& operator = (const SceneRenderObserver&) = delete;
  virtual ~SceneRenderObserver() noexcept;

private:
  void _setScene(EditorScene* newScene) noexcept;
  void _clear() noexcept;
  void _addObject(mtt::Object& object) noexcept;
  void _removeObject(mtt::Object& object) noexcept;
  void _addCulled(mtt::DrawableNode& drawable) noexcept;
  void _removeCulled(mtt::DrawableNode& drawable) noexcept;
  void _addUnculled(mtt::Drawable& drawable) noexcept;
  void _removeUnculled(mtt::Drawable& drawable) noexcept;
  void _addModificator(mtt::AreaModificator& modificator) noexcept;
  void _removeModificator(mtt::AreaModificator& modificator) noexcept;
  void _addCompositeObject(mtt::CompositeRenderObject& theObject) noexcept;
  void _removeCompositeObject(mtt::CompositeRenderObject& theObject) noexcept;

private:
  EditorCommonData& _commonData;
  mtt::RenderScene& _renderScene;

  EditorScene* _scene;

  using ObjectObservers =
                    std::vector<std::unique_ptr<AbstractObjectRenderObserver>>;
  ObjectObservers _objectObservers;
};