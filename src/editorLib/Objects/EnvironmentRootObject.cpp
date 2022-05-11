#include <memory>
#include <stdexcept>

#include <mtt/utilities/Abort.h>

#include <mtt/editorLib/Objects/EnvironmentRootObject.h>

using namespace mtt;

EnvironmentRootObject::EnvironmentRootObject( const QString& name,
                                              bool canBeRenamed,
                                              const mtt::UID& theId) :
  Object(name, canBeRenamed, theId)
{
  mtt::UID backgroundId(id().mixedUID(1096043266422623895ull));
  std::unique_ptr<mtt::BackgroundObject> background(
                                  new mtt::BackgroundObject(tr("Background"),
                                                            false,
                                                            backgroundId));
  _background = background.get();
  _background->setColor(glm::vec3(.3f, .5f, .7f));
  addSubobject(std::move(background));

  mtt::UID objectsGroupId(id().mixedUID(5000232373494366922ull));
  std::unique_ptr<mtt::EnvironmentGroup> objectsGroup(
                          new mtt::EnvironmentGroup(tr("Objects"),
                                                    false,
                                                    objectsGroupId));
  _objectsGroup = objectsGroup.get();
  addSubobject(std::move(objectsGroup));
}
