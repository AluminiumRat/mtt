#include <Objects/MeshObject.h>
#include <PropertiesWidget/VertexDataWidget.h>

VertexDataWidget::VertexDataWidget(MeshObject& mesh) :
  _mesh(mesh)
{
  setResizeMode(QListView::Adjust);
  connect(&_mesh,
          &MeshObject::geometryChanged,
          this,
          &VertexDataWidget::_updateList,
          Qt::DirectConnection);
  _updateList();
}

void VertexDataWidget::_updateList() noexcept
{
  clear();
  if(_mesh.geometry().positions.size() != 0) addItem(tr("Positions"));
  if(_mesh.geometry().normals.size() != 0) addItem(tr("Normals"));
  if(_mesh.geometry().texCoords.size() != 0) addItem(tr("Texture coordinates"));
  if(_mesh.geometry().tangents.size() != 0) addItem(tr("Tangents"));
  if(_mesh.geometry().binormals.size() != 0) addItem(tr("Binormals"));
}
