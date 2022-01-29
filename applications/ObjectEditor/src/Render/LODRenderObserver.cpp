#include <stdexcept>

#include <Objects/LODObject.h>
#include <Render/LODRenderObserver.h>

#include <mtt/utilities/Log.h>

LODRenderObserver::LODRenderObserver( LODObject& object,
                                      EditorCommonData& commonData) :
  AbstractObjectRenderObserver(object, commonData),
  _object(object)
{
  _culledDrawable.addModificator(visibleFilter());
  _culledDrawable.addModificator(_lodFilter);
  registerCulledDrawable(_culledDrawable);

  _unculledDrawable.addModificator(visibleFilter());
  _unculledDrawable.addModificator(_lodFilter);
  registerUnculledDrawable(_unculledDrawable);

  connect(&object,
          &LODObject::childAdded,
          this,
          &LODRenderObserver::_addGeometry,
          Qt::DirectConnection);

  connect(&object,
          &LODObject::childRemoved,
          this,
          &LODRenderObserver::_removeGeometry,
          Qt::DirectConnection);

  for(size_t geometryIndex = 0;
      geometryIndex < object.childsNumber();
      geometryIndex++)
  {
    _addGeometry(object.child(geometryIndex));
  }

  connect(&object,
          &LODObject::rangeChanged,
          this,
          &LODRenderObserver::_updateRange,
          Qt::DirectConnection);
  _updateRange();
}

void LODRenderObserver::_addGeometry(MeshObject& object) noexcept
{
  try
  {
    std::unique_ptr<MeshRenderObserver> newObserver(
                                  new MeshRenderObserver(object, commonData()));
    MeshRenderObserver* observerPtr = newObserver.get();
    _subobservers.push_back(std::move(newObserver));

    connect(observerPtr,
            &MeshRenderObserver::culledDrawableRegistered,
            this,
            &LODRenderObserver::_addCulled,
            Qt::DirectConnection);

    connect(observerPtr,
            &MeshRenderObserver::culledDrawableUnregistered,
            this,
            &LODRenderObserver::_removeCulled,
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
            &LODRenderObserver::_addUnculled,
            Qt::DirectConnection);

    connect(observerPtr,
            &MeshRenderObserver::unculledDrawableUnregistered,
            this,
            &LODRenderObserver::_removeUnculled,
            Qt::DirectConnection);

    for(size_t drawableIndex = 0;
        drawableIndex != observerPtr->unculledDrawablesNumber();
        drawableIndex++)
    {
      _addUnculled(observerPtr->unculledDrawable(drawableIndex));
    }
  }
  catch(std::exception& error)
  {
    mtt::Log() << error.what();
    mtt::Log() << "LODRenderObserver::_addGeometry: unable to create geometry observer";
  }
  catch(...)
  {
    mtt::Log() << "LODRenderObserver::_addGeometry: unknown error";
  }
}

void LODRenderObserver::_removeGeometry(MeshObject& object) noexcept
{
  for ( Subobservers::iterator iObserver = _subobservers.begin();
        iObserver != _subobservers.end();
        iObserver++)
  {
    MeshRenderObserver& observer = *iObserver->get();
    if (&observer.object() == &object)
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

      _subobservers.erase(iObserver);
      break;
    }
  }
}

void LODRenderObserver::_updateRange() noexcept
{
  _lodFilter.setRange(_object.minMppx(), _object.maxMppx());
}

void LODRenderObserver::_addCulled(mtt::DrawableNode& drawable) noexcept
{
  try
  {
    _culledDrawable.addNode(drawable);
  }
  catch(const std::exception& error)
  {
    mtt::Log() << "LODRenderObserver::_addCulled: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "LODRenderObserver::_addCulled: unknown error";
  }
}

void LODRenderObserver::_removeCulled(mtt::DrawableNode& drawable) noexcept
{
  try
  {
    _culledDrawable.removeNode(drawable);
  }
  catch(const std::exception& error)
  {
    mtt::Log() << "LODRenderObserver::_removeCulled: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "LODRenderObserver::_removeCulled: unknown error";
  }
}

void LODRenderObserver::_addUnculled(mtt::Drawable& drawable) noexcept
{
  try
  {
    _unculledDrawable.addChild(drawable);
  }
  catch(const std::exception& error)
  {
    mtt::Log() << "LODRenderObserver::_addUnculled: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "LODRenderObserver::_addUnculled: unknown error";
  }
}

void LODRenderObserver::_removeUnculled(mtt::Drawable& drawable) noexcept
{
  try
  {
    _unculledDrawable.removeChild(drawable);
  }
  catch(const std::exception& error)
  {
    mtt::Log() << "LODRenderObserver::_removeUnculled: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "LODRenderObserver::_removeUnculled: unknown error";
  }
}
