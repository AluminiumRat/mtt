#include <mtt/Render/SceneGraph/UnsortedField.h>

using namespace mtt;

void UnsortedField::addNode(DrawableNode& node)
{
  _area.addNode(node);
}

void UnsortedField::removeNode(DrawableNode& node) noexcept
{
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
