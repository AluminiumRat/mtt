#include <algorithm>

#include <mtt/render/SceneGraph/ProtectedGroupNode.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

void ProtectedGroupNode::setTransformMatrix(const glm::mat4& newValue) noexcept
{
  AbstractNode::setTransformMatrix(newValue);
  for(AbstractNode* child : _childs)
  {
    child->setTransformMatrix(newValue);
  }
}

bool ProtectedGroupNode::hasChild(AbstractNode& child) const noexcept
{
  return std::find(_childs.begin(), _childs.end(), &child) != _childs.end();
}

void ProtectedGroupNode::addChildProtected(AbstractNode& child)
{
  if(std::find(_childs.begin(), _childs.end(), &child) != _childs.end())
  {
    Abort("ProtectedGroupNode::addChildProtected: the child is already registered.");
  }
  _childs.push_back(&child);
  child.setTransformMatrix(transformMatrix());
}

void ProtectedGroupNode::removeChildProtected(size_t index) noexcept
{
  try
  {
    _childs.erase(_childs.begin() + index);
  }
  catch(...)
  {
    Abort("ProtectedGroupNode::removeChildProtected: unable to remove child.");
  }
}

void ProtectedGroupNode::removeChildProtected(AbstractNode& child) noexcept
{
  for(size_t index = 0; index < _childs.size(); index++)
  {
    if(_childs[index] == &child)
    {
      removeChildProtected(index);
      return;
    }
  }
}
