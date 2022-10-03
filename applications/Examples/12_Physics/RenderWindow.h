#pragma once

#include <random>

#include <mtt/application/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/clPipeline/ColorFrameBuilder.h>

class World;

class RenderWindow : public mtt::RenderWidget
{
public:
  RenderWindow(World& world);
  RenderWindow(const RenderWindow&) = delete;
  RenderWindow& operator = (const RenderWindow&) = delete;
  virtual ~RenderWindow() noexcept;

protected:
  virtual bool eventFilter(QObject* obj, QEvent* event) override;

private:
  // Calculate intersection point between the mouse click ray and the Y=0 plane
  std::optional<glm::vec2> getInsertPosition(
                                        QMouseEvent& mouseEvent) const noexcept;

private:
  mtt::OrbitalCameraController _cameraController;
  mtt::clPipeline::ColorFrameBuilder _frameBuilder;
  mtt::CameraNode _camera;

  World& _world;

  std::default_random_engine _randomEngine;
  std::uniform_real_distribution<float> _rotationDistribution;
  std::uniform_real_distribution<float> _sizeDistribution;
};