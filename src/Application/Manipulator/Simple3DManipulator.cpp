#include <vector>

#include <glm/gtx/transform.hpp>
#include <glm/vec2.hpp>

#include <mtt/Application/Manipulator/Simple3DManipulator.h>
#include <mtt/Application/Application.h>
#include <mtt/Render/SceneGraph/CameraNode.h>
#include <mtt/Utilities/Log.h>
#include <mtt/Utilities/Segment.h>

using namespace mtt;

Simple3DManipulator::Simple3DManipulator(
                                AutoscaleDrawableModificator::Mode scaleMode) :
  _scaleMode(scaleMode),
  _mesh(Application::instance().displayDevice()),
  _unselectedColor(1.f, 1.f, 1.f, .2f),
  _highlihtedColor(1.f, 1.f, 1.f, .4f),
  _activatedColor(1.f, 1.f, 1.f, .8f)
{
  _autoscale.setMode(_scaleMode);
  _mesh.registedModificator(_autoscale);
  _node.setDrawable(&_mesh, Sphere());

  _updateColor();
}

void Simple3DManipulator::setGeometry(const std::vector<glm::vec3>& vertices)
{
  _surface.setVertices(vertices);

  if(!vertices.empty())
  {
    std::shared_ptr<Buffer> positionsBuffer(
                            new Buffer( Application::instance().displayDevice(),
                                        Buffer::VERTEX_BUFFER));
    positionsBuffer->setData( vertices.data(),
                              sizeof(glm::vec3) * vertices.size());
    _mesh.setPositionBuffer(positionsBuffer);
    _mesh.setVerticesNumber(int(vertices.size()));
  }
  else
  {
    _mesh.removePositionBuffer();
    _mesh.setVerticesNumber(0);
  }
}

void Simple3DManipulator::setUnselectedColor(const glm::vec4& newValue)
{
  if(newValue == _unselectedColor) return;
  _unselectedColor = newValue;
  _updateColor();
}

void Simple3DManipulator::setHighlihtedColor(const glm::vec4& newValue)
{
  if (newValue == _highlihtedColor) return;
  _highlihtedColor = newValue;
  _updateColor();
}

void Simple3DManipulator::setActivatedColor(const glm::vec4& newValue)
{
  if (newValue == _activatedColor) return;
  _activatedColor = newValue;
  _updateColor();
}

void Simple3DManipulator::_updateColor()
{
  glm::vec4 color;
  switch(state())
  {
  case UNSELECTED:
    color = _unselectedColor;
    break;
  case HIGHLITED:
    color = _highlihtedColor;
    break;
  case ACTIVATED:
    color = _activatedColor;
    break;
  default:
    color = _unselectedColor;
  }

  SurfaceMaterialData material;
  material.albedo = color;
  material.opaque = color.a;
  _mesh.extraData().setSurfaceMaterialData(material);
}

void Simple3DManipulator::processNewState(State newState) noexcept
{
  _updateColor();
}

std::optional<glm::vec3> Simple3DManipulator::intersect(
                                      const TouchInfo& touchInfo) const noexcept
{
  glm::vec4 manipulatorPosition =
                                _node.transformMatrix() * glm::vec4(0, 0, 0, 1);
  glm::vec3 viewSpacePosition = touchInfo.camera.viewMatrix() *
                                                            manipulatorPosition;
  ViewInfo viewInfo = touchInfo.camera.buildViewInfo(touchInfo.frameSize);
  float scale = AutoscaleDrawableModificator::scale(viewSpacePosition,
                                                    viewInfo,
                                                    _scaleMode);
  if(scale == 0) return std::nullopt;
  glm::mat4 manipulatorToWorld =
                        _node.transformMatrix() * glm::scale(glm::vec3(scale));
  glm::mat4 worldToManipulator = glm::inverse(manipulatorToWorld);

  glm::vec3 mouseDirection = touchInfo.camera.getDirection(
                                                  touchInfo.touchPositionNorm);
  Segment mouseSegment;
  mouseSegment.p1 = worldToManipulator *
                                      glm::vec4(touchInfo.camera.eyePoint(),1);
  mouseSegment.p2 = worldToManipulator *
        glm::vec4(touchInfo.camera.eyePoint() +
                            mouseDirection * touchInfo.camera.farDistance(), 1);

  std::optional<glm::vec3> intersection =
                                    _surface.nearestIntersection(mouseSegment);
  if(!intersection.has_value()) return std::nullopt;
  return glm::vec3(manipulatorToWorld * glm::vec4(intersection.value(), 1));
}

float Simple3DManipulator::checkTouchPosition(
                                      const TouchInfo& touchInfo) const noexcept
{
  std::optional<glm::vec3> intersecPoint = intersect(touchInfo);
  if(!intersecPoint.has_value()) return 0;
  else return glm::length(intersecPoint.value() - touchInfo.camera.eyePoint());
}

std::optional<glm::vec3> Simple3DManipulator::tryActivate(
                                            const TouchInfo& touchInfo) noexcept
{
  std::optional<glm::vec3> intersecPoint = intersect(touchInfo);
  if(intersecPoint.has_value())
  {
    setActivated(true);
    return intersecPoint;
  }
  else return std::nullopt;
}

void Simple3DManipulator::release() noexcept
{
  Manipulator3D::release();
}

mtt::Drawable& Simple3DManipulator::drawable() noexcept
{
  return _node;
}

mtt::AbstractNode& Simple3DManipulator::node() noexcept
{
  return _node;
}
