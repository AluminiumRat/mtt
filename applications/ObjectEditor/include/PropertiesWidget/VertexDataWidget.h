#pragma once

#include <QtWidgets/QListWidget>

class MeshObject;

class VertexDataWidget : public QListWidget
{
  Q_OBJECT

public:
  VertexDataWidget(MeshObject& mesh);
  VertexDataWidget(const VertexDataWidget&) = delete;
  VertexDataWidget& operator = (const VertexDataWidget&) = delete;
  virtual ~VertexDataWidget() noexcept = default;

private:
  void _updateList() noexcept;

private:
  MeshObject& _mesh;
};