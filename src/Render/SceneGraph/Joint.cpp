#include <mtt/Render/SceneGraph/Joint.h>

using namespace mtt;

Joint::Joint() noexcept :
  _jointMatrix(1)
{
}

void Joint::setTransformMatrix(const glm::mat4& newValue) noexcept
{
  AbstractNode::setTransformMatrix(newValue);
  _updateChilds();
}

void Joint::setJointMatrix(const glm::mat4& newValue) noexcept
{
  _jointMatrix = newValue;
  _updateChilds();
}

void Joint::silentSetJointMatrix(const glm::mat4& newValue) noexcept
{
  _jointMatrix = newValue;
}

void Joint::_updateChilds() noexcept
{
  glm::mat4 childTransform = transformMatrix() * _jointMatrix;

  for(size_t childIndex = 0;
      childIndex < childNumber();
      childIndex++)
  {
    child(childIndex).setTransformMatrix(childTransform);
  }
}

void Joint::addChild(AbstractNode& child)
{
  GroupNode::addChild(child);
  child.setTransformMatrix(transformMatrix() * _jointMatrix);
}
