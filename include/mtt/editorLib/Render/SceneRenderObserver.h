#pragma once

#include <memory>
#include <vector>

#include <QtCore/QObject>

#include <mtt/editorLib/Render/AbstractObjectRenderObserver.h>

namespace mtt
{
  class Object;
  class RenderObserverFactory;
  class RenderScene;
  class Scene;

  class SceneRenderObserver : public QObject
  {
    Q_OBJECT

  public:
    explicit SceneRenderObserver( Scene& scene,
                                  RenderScene& renderScene,
                                  RenderObserverFactory& observerFactory);
    SceneRenderObserver(const SceneRenderObserver&) = delete;
    SceneRenderObserver& operator = (const SceneRenderObserver&) = delete;
    virtual ~SceneRenderObserver() noexcept;

  private:
    void _addObject(Object& object) noexcept;
    void _removeObject(Object& object) noexcept;
    void _addCulled(DrawableNode& drawable) noexcept;
    void _removeCulled(DrawableNode& drawable) noexcept;
    void _addUnculled(Drawable& drawable) noexcept;
    void _removeUnculled(Drawable& drawable) noexcept;
    void _addModificator(AreaModificator& modificator) noexcept;
    void _removeModificator(AreaModificator& modificator) noexcept;
    void _addCompositeObject(CompositeRenderObject& theObject) noexcept;
    void _removeCompositeObject(CompositeRenderObject& theObject) noexcept;

  private:
    Scene& _scene;
    RenderScene& _renderScene;
    RenderObserverFactory& _observerFactory;

    using ObjectObservers =
                    std::vector<std::unique_ptr<AbstractObjectRenderObserver>>;
    ObjectObservers _objectObservers;
  };
}