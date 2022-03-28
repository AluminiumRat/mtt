#include <mtt/utilities/Log.h>

#include <Objects/BlockerObject.h>
#include <Render/BlockerRenderObserver.h>

#define ICON_FILE ":/particlesEditor/blocker.png"
#define ICON_SIZE 32

#define CAP_SEGMENTS 32
#define BODY_SEGMENTS 8

BlockerRenderObserver::BlockerRenderObserver( BlockerObject& object,
                                              mtt::CommonEditData& commonData) :
  Object3DRenderObserver(object, commonData),
  _object(object),
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

  connect(&_object,
          &BlockerObject::shapeChanged,
          this,
          &BlockerRenderObserver::_updateHull,
          Qt::DirectConnection);

  connect(&_object,
          &BlockerObject::sizeChanged,
          this,
          &BlockerRenderObserver::_updateHull,
          Qt::DirectConnection);

  _updateHull();
}

void BlockerRenderObserver::_updateHull() noexcept
{
  if (_object.size() <= 0.f)
  {
    _hullNode.resetGeometry();
    return;
  }

  try
  {
    if (_object.shape() == BlockerObject::SPHERE_SHAPE)
    {
      _hullNode.setSphereGeometry(_object.size() / 2.f, CAP_SEGMENTS);
    }
    else if(_object.shape() == BlockerObject::BOX_SHAPE)
    {
      _hullNode.setBoxGeometry(glm::vec3(_object.size()));
    }
    else
    {
      _hullNode.setCylinderGeometry(_object.size() / 2.f,
                                    _object.size(),
                                    0.f,
                                    BODY_SEGMENTS,
                                    CAP_SEGMENTS);
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