#include <atomic>

#include <mtt/application/Scene/ObjectGroup.h>
#include <mtt/application/Scene/ObjectVisitor.h>
#include <mtt/application/Scene/Object.h>

using namespace mtt;

ObjectVisitor::ExtensionID ObjectVisitor::registerExtension() noexcept
{
  static std::atomic<ExtensionID> counter;
  return ++counter;
}

void ObjectVisitor::process(const Object& object)
{
  object.accept(*this);
}

void ObjectVisitor::process(Object& object)
{
  object.accept(*this);
}

void ObjectVisitor::visitConstObject(const Object& object)
{
}

void ObjectVisitor::visitObject(Object& object)
{
}

void ObjectVisitor::visitConstObjectGroup(const ObjectGroup& object)
{
  visitConstObject(object);
}

void ObjectVisitor::visitObjectGroup(ObjectGroup& object)
{
  visitObject(object);
}
