#include <Objects/EmitterObject.h>
#include <Render/EmitterRenderObserver.h>

#define ICON_FILE ":/particlesEditor/emitter.png"
#define ICON_SIZE 32

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
}
