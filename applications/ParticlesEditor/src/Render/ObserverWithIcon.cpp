#include <Render/ObserverWithIcon.h>

ObserverWithIcon::ObserverWithIcon( mtt::Object3D& object,
                                    mtt::CommonEditData& commonData,
                                    const QString& iconFilename,
                                    float iconSize) :
  Object3DRenderObserver(object, commonData)
{
  if (!iconFilename.isEmpty() && iconSize > 0)
  {
    _iconNode.emplace(iconFilename, iconSize);
    registerUnculledDrawable(*_iconNode);
    positionJoint().addChild(*_iconNode);
    _iconNode->addModificator(visibleFilter());
    _iconNode->addModificator(uidSetter());
    _iconNode->addModificator(selectionModificator());
  }
}
