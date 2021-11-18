#include <algorithm>

#include <mtt/Render/SceneGraph/GroupNode.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

void GroupNode::setTransformMatrix(const glm::mat4& newValue) noexcept
{
  AbstractNode::setTransformMatrix(newValue);
  for(AbstractNode* child : _childs)
  {
    child->setTransformMatrix(newValue);
  }
}

bool GroupNode::hasChild(AbstractNode& child) const noexcept
{
  return std::find(_childs.begin(), _childs.end(), &child) != _childs.end();
}

void GroupNode::addChild(AbstractNode& child)
{
  if(std::find(_childs.begin(), _childs.end(), &child) != _childs.end())
  {
    Abort("GroupNode::addChild: the child is already registered.");
  }
  _childs.push_back(&child);
  child.setTransformMatrix(transformMatrix());
}

void GroupNode::removeChild(size_t index) noexcept
{
  try
  {
    _childs.erase(_childs.begin() + index);
  }
  catch(...)
  {
    Abort("GroupNode::removeChild: unable to remove child.");
  }
}

void GroupNode::removeChild(AbstractNode& child) noexcept
{
  for(size_t index = 0; index < _childs.size(); index++)
  {
    if(_childs[index] == &child)
    {
      removeChild(index);
      return;
    }
  }
}
