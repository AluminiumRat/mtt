#include <Objects/Fluid/FluidModificator.h>
#include <Render/FluidModificatorObserver.h>

#define ICON_SIZE 32

#define CAP_SEGMENTS 32

FluidModificatorObserver::FluidModificatorObserver(
                                            FluidModificator& object,
                                            const QString& iconFilename,
                                            mtt::CommonEditData& commonData) :
  Object3DRenderObserver(object, commonData),
  _object(object),
  _iconNode(iconFilename, ICON_SIZE)
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

  connect(&_object,
          &FluidModificator::sizeChanged,
          this,
          &FluidModificatorObserver::_updateHull,
          Qt::DirectConnection);

  _updateHull();
}

void FluidModificatorObserver::_updateHull() noexcept
{
  try
  {
    if (_object.size() <= 0.f)
    {
      _hullNode.resetGeometry();
      return;
    }

    _hullNode.setSphereGeometry(_object.size() / 2.f, CAP_SEGMENTS);
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