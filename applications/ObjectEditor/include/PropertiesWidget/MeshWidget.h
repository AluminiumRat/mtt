#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <Objects/MeshObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class MeshWidget;
}

class MaterialWidget;

class MeshWidget : public QWidget
{
public:
  MeshWidget( MeshObject& object,
              mtt::Object& materialsGroup,
              mtt::UndoStack& undoStack);
  MeshWidget(const MeshWidget&) = delete;
  MeshWidget& operator = (const MeshWidget&) = delete;
  virtual ~MeshWidget() noexcept;

private:
  void _updateMaterialWidget() noexcept;

private:
  std::unique_ptr<Ui::MeshWidget> _ui;
  MeshObject& _object;
  mtt::UndoStack& _undoStack;
  MaterialWidget* _materialWidget;
};
