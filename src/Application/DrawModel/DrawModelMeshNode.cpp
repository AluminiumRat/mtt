#include <mtt/Application/DrawModel/DrawModelMeshNode.h>

using namespace mtt;

DrawModelMeshNode::DrawModelMeshNode(LogicalDevice& device) :
  _mesh(device)
{
  _mesh.registedModificator(_lodFilter);
  setDrawable(&_mesh, Sphere());
}
