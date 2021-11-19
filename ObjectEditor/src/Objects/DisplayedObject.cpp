#include <mtt/Core/Scene/object_cast.h>

#include <Objects/DisplayedObject.h>
#include <Objects/OEVisitor.h>

DisplayedObject::DisplayedObject(const mtt::UID& id) :
  Object(id),
  _visible(true),
  _parentVisible(true),
  _completeVisible(true)
{
}

void DisplayedObject::setVisible(bool newValue) noexcept
{
  if(newValue == _visible) return;
  _visible = newValue;
  _updateCompleteVisible();
  emit visibleChanged(_visible);
}

void DisplayedObject::setParentVisible(bool newValue) noexcept
{
  if(newValue == _parentVisible) return;
  _parentVisible = newValue;
  _updateCompleteVisible();
}

void DisplayedObject::_updateCompleteVisible() noexcept
{
  bool newValue = _visible && _parentVisible;
  if(newValue == _completeVisible) return;
  _completeVisible = newValue;

  for(size_t childIndex = 0;
      childIndex < subobjectNumber();
      childIndex++)
  {
    mtt::Object& child = subobject(childIndex);
    DisplayedObject* displayedChild =
                            mtt::object_cast<DisplayedObject, OEVisitor>(child);
    if(displayedChild != nullptr) displayedChild->setParentVisible(newValue);
  }

  emit completeVisibleChanged(_completeVisible);
}

void DisplayedObject::onSubobjectAdded(Object& object) noexcept
{
  DisplayedObject* displayedChild =
                          mtt::object_cast<DisplayedObject, OEVisitor>(object);
  if(displayedChild != nullptr)
  {
    displayedChild->setParentVisible(completeVisible());
  }

  Object::onSubobjectAdded(object);
}

void DisplayedObject::onSubobjectRemoved(Object& object) noexcept
{
  DisplayedObject* displayedChild =
                          mtt::object_cast<DisplayedObject, OEVisitor>(object);
  if (displayedChild != nullptr) displayedChild->setParentVisible(true);

  Object::onSubobjectRemoved(object);
}
