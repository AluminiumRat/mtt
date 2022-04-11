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
  _objects = objectsGroup.get();
  addSubobject(std::move(objectsGroup));
}

void EnvironmentRootObject::changeBackground(
                            std::unique_ptr<mtt::BackgroundObject> newBackground)
{
  if(newBackground == nullptr) mtt::Abort("RootObject::changeBackground: newBackground is nullpt.");

  std::unique_ptr<mtt::Object> oldBackgraund = removeSubobject( *_background,
                                                                false);
  mtt::BackgroundObject* newBackgroundPtr = newBackground.get();
  try
  {
    addSubobject(std::move(newBackground));
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    mtt::Abort("RootObject::changeBackground: unable to change background.");
  }
  catch (...)
  {
    mtt::Abort("RootObject::changeBackground: unable to change background.");
  }

  _background = newBackgroundPtr;
  emit backgroundChanged(*_background);
}
