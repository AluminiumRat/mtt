#include <mtt/Application/Scene/ObjectLink.h>

using namespace mtt;

ObjectRefBase::ObjectRefBase(const UID& referencedId) :
  _referencedPtr(nullptr),
  _referencedId(referencedId)
{
}

void ObjectRefBase::setReferencedPtr(Object* referenced)
{
  if(_referencedPtr == referenced) return;

  Object* oldReferenced = _referencedPtr;
  _referencedPtr = nullptr;
  if(oldReferenced != nullptr) unlink(*oldReferenced);

  _referencedPtr = referenced;
  try
  {
    if(_referencedPtr != nullptr) link(*_referencedPtr);
  }
  catch(...)
  {
    _referencedPtr = nullptr;
  }
}
