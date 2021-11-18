#include <mtt/Render/SceneGraph/AbstractField.h>
#include <mtt/Render/SceneGraph/DrawableNode.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

void AbstractField::addNode(DrawableNode& node)
{
  if(node.field() == this) Abort("AbstractField::addNode: the node is already registered in this field.");
  if(node.field() != nullptr) node.field()->removeNode(node);
  node.setField(this);
}

void AbstractField::removeNode(DrawableNode& node) noexcept
{
  if (node.field() != this) Abort("AbstractField::addNode: the node is not registered in this field.");
  node.setField(nullptr);
}
