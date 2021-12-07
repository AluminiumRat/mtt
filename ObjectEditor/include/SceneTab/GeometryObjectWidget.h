#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <Objects/GeometryObject.h>

namespace Ui
{
  class GeometryObjectWidget;
}

namespace mtt
{
  class UndoStack;
}

class GeometryObjectWidget : public QWidget
{
public:
  GeometryObjectWidget( GeometryObject& object,
                        mtt::Object& skeletonGroup,
                        mtt::UndoStack& undoStack);
  GeometryObjectWidget(const GeometryObjectWidget&) = delete;
  GeometryObjectWidget& operator = (const GeometryObjectWidget&) = delete;
  virtual ~GeometryObjectWidget() noexcept;

private:
  std::unique_ptr<Ui::GeometryObjectWidget> _ui;
};
