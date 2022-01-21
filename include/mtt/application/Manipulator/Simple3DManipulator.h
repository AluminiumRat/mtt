#pragma once

#include <optional>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <mtt/application/Manipulator/Manipulator3D.h>
#include <mtt/render/Drawable/AutoscaleDrawableModificator.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>
#include <mtt/utilities/Surface.h>


namespace mtt
{
  /// Translate along local z axis
  class Simple3DManipulator : public Manipulator3D
  {
  public:
    explicit Simple3DManipulator(AutoscaleDrawableModificator::Mode scaleMode);
    Simple3DManipulator(const Simple3DManipulator&) = delete;
    Simple3DManipulator& operator = (const Simple3DManipulator&) = delete;
    virtual ~Simple3DManipulator() noexcept = default;

    virtual float checkTouchPosition(
                            const TouchInfo& touchInfo) const noexcept override;

    virtual std::optional<glm::vec3> tryActivate(
                                  const TouchInfo& touchInfo) noexcept override;
    virtual void release() noexcept override;

    virtual Drawable& drawable() noexcept override;
    virtual AbstractNode& node() noexcept override;

    inline const glm::vec4& unselectedColor() const noexcept;
    void setUnselectedColor(const glm::vec4& newValue);

    inline const glm::vec4& highlihtedColor() const noexcept;
    void setHighlihtedColor(const glm::vec4& newValue);

    inline const glm::vec4& activatedColor() const noexcept;
    void setActivatedColor(const glm::vec4& newValue);

  protected:
    void setGeometry(const std::vector<glm::vec3>& vertices);
    inline void setTechnique(
                          FrameType frameType,
                          std::unique_ptr<AbstractMeshTechnique> newTechnique);

    std::optional<glm::vec3> intersect(
                                    const TouchInfo& touchInfo) const noexcept;

    virtual void processNewState(State newState) noexcept override;

  private:
    void _updateColor();

  private:
    Surface _surface;

    AutoscaleDrawableModificator::Mode _scaleMode;
    AutoscaleDrawableModificator _autoscale;
    Mesh _mesh;
    SimpleDrawableNode _node;

    glm::vec4 _unselectedColor;
    glm::vec4 _highlihtedColor;
    glm::vec4 _activatedColor;
  };

  inline const glm::vec4& Simple3DManipulator::unselectedColor() const noexcept
  {
    return _unselectedColor;
  }

  inline const glm::vec4& Simple3DManipulator::highlihtedColor() const noexcept
  {
    return _highlihtedColor;
  }

  inline const glm::vec4& Simple3DManipulator::activatedColor() const noexcept
  {
    return _activatedColor;
  }

  inline void Simple3DManipulator::setTechnique(
                            FrameType frameType,
                            std::unique_ptr<AbstractMeshTechnique> newTechnique)
  {
    _mesh.setTechnique(frameType, std::move(newTechnique));
  }
}