#pragma once

#include <QtCore/QObject>

#include <mtt/Application/Manipulator/AbstractManipulator.h>
#include <mtt/Render/Drawable/VisibleDrawableFilter.h>

class QMouseEvent;

namespace mtt
{
  class CameraNode;
  class RenderWidget;
  class RenderScene;

  class ManipulatorController : public QObject
  {
    Q_OBJECT

  public:
    explicit ManipulatorController(RenderWidget& widget);
    ManipulatorController(const ManipulatorController&) = delete;
    ManipulatorController& operator = (const ManipulatorController&) = delete;
    virtual ~ManipulatorController() noexcept;

    virtual bool eventFilter(QObject* obj, QEvent* event) override;

  protected:
    void setManipulator(AbstractManipulator* newManipulator);
    void resetManipulator() noexcept;

  private:
    void _setCamera(CameraNode* camera) noexcept;
    void _setRenderScene(RenderScene* newScene) noexcept;
    AbstractManipulator::TouchInfo _touchInfo(
                                        QMouseEvent& mouseEvent) const noexcept;

  private:
    RenderWidget& _widget;
    CameraNode* _camera;
    RenderScene* _renderScene;
    AbstractManipulator* _currentManipulator;
    Drawable* _manipulatorDrawable;
  };
}