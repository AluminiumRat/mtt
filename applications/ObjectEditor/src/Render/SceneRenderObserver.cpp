#include <stdexcept>

#include <mtt/utilities/Log.h>

#include <Objects/OEVisitor.h>
#include <Render/AmbientLightRenderObserver.h>
#include <Render/BackgroundObserver.h>
#include <Render/DirectLightRenderObserver.h>
#include <Render/EnvironmentModelRenderObserver.h>
#include <Render/LODRenderObserver.h>
#include <Render/SceneRenderObserver.h>
#include <Render/SkeletonRenderObserver.h>
#include <EditorCommonData.h>

SceneRenderObserver::SceneRenderObserver(EditorCommonData& commonData) :
  _commonData(commonData),
  _renderScene(_commonData.renderScene()),
  _scene(nullptr)
{
  connect(&_commonData,
          &EditorCommonData::sceneChanged,
          this,
          &SceneRenderObserver::_setScene,
          Qt::DirectConnection);
  _setScene(_commonData.scene());
}

SceneRenderObserver::~SceneRenderObserver() noexcept
{
  while (!_objectObservers.empty())
  {
    _removeObject(_objectObservers.front()->object());
  }
}

void SceneRenderObserver::_setScene(EditorScene* newScene) noexcept
{
  if(_scene != nullptr)
  {
    disconnect( _scene,
                &EditorScene::objectRegistered,
                this,
                &SceneRenderObserver::_addObject);

    disconnect( _scene,
                &EditorScene::objectUnregistered,
                this,
                &SceneRenderObserver::_removeObject);
  }

  _clear();
  _scene = newScene;

  if(_scene != nullptr)
  {
    connect(_scene,
            &EditorScene::objectRegistered,
            this,
            &SceneRenderObserver::_addObject,
            Qt::DirectConnection);

    connect(_scene,
            &EditorScene::objectUnregistered,
            this,
            &SceneRenderObserver::_removeObject,
            Qt::DirectConnection);

    try
    {
      auto addDelegate =  [&](mtt::Object& theObject)
                          {
                            _addObject(theObject);
                          };
      _scene->processObjects(addDelegate);
    }
    catch (std::exception& error)
    {
      mtt::Log() << "SceneRenderObserver::_setScene: " << error.what();
    }
    catch (...)
    {
      mtt::Log() << "SceneRenderObserver::_setScene: unknown error.";
    }
  }
}

void SceneRenderObserver::_clear() noexcept
{
  _objectObservers.clear();
}

void SceneRenderObserver::_addObject(mtt::Object& object) noexcept
{
  struct ObserverBuilder : public OEVisitor
  {
    EditorCommonData* commonData;
    std::unique_ptr<AbstractObjectRenderObserver> result;

    virtual void visit(AmbientLightObject& object) override
    {
      result.reset(new AmbientLightRenderObserver(object, *commonData));
    }

    virtual void visit(BackgroundObject& object) override
    {
      result.reset(new BackgroundObserver(object, *commonData));
    }

    virtual void visit(DirectLightObject& object) override
    {
      result.reset(new DirectLightRenderObserver(object, *commonData));
    }

    virtual void visit(EnvironmentModel& object) override
    {
      result.reset(new EnvironmentModelRenderObserver(object, *commonData));
    }

    virtual void visit(SkeletonObject& object) override
    {
      result.reset(new SkeletonRenderObserver(object, *commonData));
    }

    virtual void visit(LODObject& object) override
    {
      result.reset(new LODRenderObserver(object, *commonData));
    }
  } observerBuilder;
  observerBuilder.commonData = &_commonData;
  
  try
  {
    observerBuilder.process(object);

    AbstractObjectRenderObserver* observerPtr = observerBuilder.result.get();
    if (observerPtr != nullptr)
    {
      _objectObservers.push_back(std::move(observerBuilder.result));

      connect(observerPtr,
              &MeshRenderObserver::culledDrawableRegistered,
              this,
              &SceneRenderObserver::_addCulled,
              Qt::DirectConnection);

      connect(observerPtr,
              &MeshRenderObserver::culledDrawableUnregistered,
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
              &MeshRenderObserver::unculledDrawableRegistered,
              this,
              &SceneRenderObserver::_addUnculled,
              Qt::DirectConnection);

      connect(observerPtr,
              &MeshRenderObserver::unculledDrawableUnregistered,
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
              &MeshRenderObserver::areaModificatorRegistered,
              this,
              &SceneRenderObserver::_addModificator,
              Qt::DirectConnection);

      connect(observerPtr,
              &MeshRenderObserver::areaModificatorUnregistered,
              this,
              &SceneRenderObserver::_removeModificator,
              Qt::DirectConnection);

      for(size_t modificatorIndex = 0;
          modificatorIndex != observerPtr->areaModificatorsNumber();
          modificatorIndex++)
      {
        _addModificator(observerPtr->areaModificator(modificatorIndex));
      }
    }
  }
  catch(std::exception& error)
  {
    mtt::Log() << "SceneRenderObserver::_addObject: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "SceneRenderObserver::_addObject: unknown error.";
  }
}

void SceneRenderObserver::_removeObject(mtt::Object& object) noexcept
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

      _objectObservers.erase(iObserver);
      break;
    }
  }
}

void SceneRenderObserver::_addCulled(mtt::DrawableNode& drawable) noexcept
{
  try
  {
    _commonData.renderScene().addCulledDrawable(drawable);
  }
  catch(const std::exception& error)
  {
    mtt::Log() << "SceneRenderObserver::_addCulled: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "SceneRenderObserver::_addCulled: unknown error";
  }
}

void SceneRenderObserver::_removeCulled(mtt::DrawableNode& drawable) noexcept
{
  _commonData.renderScene().removeCulledDrawable(drawable);
}

void SceneRenderObserver::_addUnculled(mtt::Drawable& drawable) noexcept
{
  try
  {
    _commonData.renderScene().addUnculledDrawable(drawable);
  }
  catch(const std::exception& error)
  {
    mtt::Log() << "SceneRenderObserver::_addUnculled: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "SceneRenderObserver::_addUnculled: unknown error";
  }
}

void SceneRenderObserver::_removeUnculled(mtt::Drawable& drawable) noexcept
{
  _commonData.renderScene().removeUnculledDrawable(drawable);
}

void SceneRenderObserver::_addModificator(
                                    mtt::AreaModificator& modificator) noexcept
{
  try
  {
    _commonData.renderScene().addAreaModificator(modificator);
  }
  catch(const std::exception& error)
  {
    mtt::Log() << "SceneRenderObserver::_addModificator: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "SceneRenderObserver::_addModificator: unknown error";
  }
}

void SceneRenderObserver::_removeModificator(
                                    mtt::AreaModificator& modificator) noexcept
{
  _commonData.renderScene().removeAreaModificator(modificator);
}
