#include <mtt/render/SceneGraph/UnsortedField.h>

using namespace mtt;

void UnsortedField::addDrawable(DrawableNode& node)
{
  _area.addDrawable(node);
}

void UnsortedField::removeDrawable(DrawableNode& node) noexcept
{
  _area.removeDrawable(node);
}

void UnsortedField::enumerateAreas( Visitor& visitor,
                                    const ViewFrustum& frustum) const
{
  visitor.visit(_area);
}
