#include <mtt/Application/DrawModel/DrawModelMeshNode.h>

using namespace mtt;

DrawModelMeshNode::DrawModelMeshNode(LogicalDevice& device) :
  _mesh(device)
{
  _mesh.registerModificator(_lodFilter);
  setDrawable(&_mesh, Sphere());
}
