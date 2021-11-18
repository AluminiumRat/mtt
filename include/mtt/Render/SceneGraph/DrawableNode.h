#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/Render/Drawable/Drawable.h>
#include <mtt/Render/SceneGraph/AbstractNode.h>
#include <mtt/Utilities/Abort.h>
#include <mtt/Utilities/Sphere.h>

namespace mtt
{
  class AbstractField;
  struct DrawPlanBuildInfo;

  class DrawableNode :  public AbstractNode,
                        public Drawable
  {
  public:
    DrawableNode() noexcept;
    DrawableNode(const DrawableNode&) = delete;
    DrawableNode& operator = (const DrawableNode&) = delete;
    virtual ~DrawableNode() = default;

    inline AbstractField* field() const noexcept;

    inline const Sphere& localBoundSphere() const noexcept;
    inline const Sphere& transformedBoundSphere() const noexcept;

    virtual void setTransformMatrix(
                                  const glm::mat4& newValue) noexcept override;

    virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) override;

  protected:
    virtual void setLocalBoundSphere(const Sphere& newValue) noexcept;

  private:
    void updateTransformedBoundSphere() noexcept;

    friend class AbstractField;
    inline void setField(AbstractField* newField) noexcept;

  private:
    Sphere _localBoundSphere;
    Sphere _transformedBoundSphere;

    glm::mat3 _normalTransformMatrix;

    AbstractField* _field;
  };

  inline AbstractField* DrawableNode::field() const noexcept
  {
    return _field;
  }

  inline const Sphere& DrawableNode::localBoundSphere() const noexcept
  {
    return _localBoundSphere;
  }

  inline const Sphere& DrawableNode::transformedBoundSphere() const noexcept
  {
    return _transformedBoundSphere;
  }

  inline void DrawableNode::setField(AbstractField* newField) noexcept
  {
    if(_field != nullptr && newField != nullptr) Abort("DrawableNode::setField: the node is already registered in field.");
    _field = newField;
  }
}