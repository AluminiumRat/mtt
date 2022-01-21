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

void ObjectVisitor::visit(const Object& object)
{
}

void ObjectVisitor::visit(Object& object)
{
}

#define DEFINE_VISIT(ObjectClass, ParentClass)                                \
void ObjectVisitor::visit(const ObjectClass& object)                          \
{                                                                             \
  visit(static_cast<const ParentClass&>(object));                             \
}                                                                             \
void ObjectVisitor::visit(ObjectClass& object)                                \
{                                                                             \
  visit(static_cast<ParentClass&>(object));                                   \
}

DEFINE_VISIT(ObjectGroup, Object);
