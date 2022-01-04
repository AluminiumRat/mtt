#include <mtt/Render/SceneGraph/UnsortedField.h>

using namespace mtt;

void UnsortedField::addNode(DrawableNode& node)
{
  _area.addNode(node);
  try
  {
    VisitedField::addNode(node);
  }
  catch (...)
  {
    _area.removeNode(node);
    throw;
  }
}

void UnsortedField::removeNode(DrawableNode& node) noexcept
{
  VisitedField::removeNode(node);
  _area.removeNode(node);
}

void UnsortedField::onNodeBoundChanged( DrawableNode& node,
                                        const Sphere& oldBound) noexcept
{
}

void UnsortedField::enumerateAreas( Visitor& visitor,
                                    const ViewFrustum& frustum) const
{
  visitor.visit(_area);
}
