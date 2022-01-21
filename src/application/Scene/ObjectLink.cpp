#include <mtt/application/Scene/ObjectLink.h>

using namespace mtt;

ObjectRefBase::ObjectRefBase() :
  _referencedPtr(nullptr),
  _referencedId(UID())
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
    throw;
  }
}
