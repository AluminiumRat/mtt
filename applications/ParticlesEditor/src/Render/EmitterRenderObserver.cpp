#include <mtt/utilities/Log.h>

#include <Objects/EmitterObject.h>
#include <Render/EmitterRenderObserver.h>

#define ICON_FILE ":/particlesEditor/emitter.png"
#define ICON_SIZE 32

#define HULL_SEGMENTS 32

EmitterRenderObserver::EmitterRenderObserver( EmitterObject& object,
                                              mtt::CommonEditData& commonData) :
  Object3DRenderObserver(object, commonData),
  _emitter(object),
  _iconNode(ICON_FILE, ICON_SIZE)
{
  registerUnculledDrawable(_iconNode);
  positionJoint().addChild(_iconNode);
  _iconNode.addModificator(visibleFilter());
  _iconNode.addModificator(uidSetter());
  _iconNode.addModificator(selectionModificator());

  registerCulledDrawable(_hullNode);
  fullTransformJoint().addChild(_hullNode);
  _hullNode.addModificator(visibleFilter());
  _hullNode.addModificator(uidSetter());
  _hullNode.addModificator(selectionModificator());

  connect(&_emitter,
          &EmitterObject::shapeChanged,
          this,
          &EmitterRenderObserver::_updateHull,
          Qt::DirectConnection);

  connect(&_emitter,
          &EmitterObject::sizeChanged,
          this,
          &EmitterRenderObserver::_updateHull,
          Qt::DirectConnection);

  _updateHull();
}

void EmitterRenderObserver::_updateHull() noexcept
{
  if (_emitter.size() <= 0.f)
  {
    _hullNode.resetGeometry();
    return;
  }

  try
  {
    if(_emitter.shape() == EmitterObject::SPHERE_SHAPE)
    {
      _hullNode.setSphereGeometry(_emitter.size() / 2.f, HULL_SEGMENTS);
    }
    else
    {
      _hullNode.setCircleGeometry(_emitter.size() / 2.f, HULL_SEGMENTS);
    }
  }
  catch (std::exception& error)
  {
    mtt::Log() << "EmitterRenderObserver::_updateHull: unable to update hull node: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "EmitterRenderObserver::_updateHull: unable to update hull node: unknown error.";
  }
}
