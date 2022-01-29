#pragma once

#include <vector>

#include <glm/mat4x4.hpp>

#include <mtt/render/Drawable/DrawableModificator.h>

namespace mtt
{
  class BoneMatricesSetter : public DrawableModificator
  {
  public:
    BoneMatricesSetter() = default;
    BoneMatricesSetter(const BoneMatricesSetter&) = delete;
    BoneMatricesSetter& operator = (
      const BoneMatricesSetter&) = delete;
    virtual ~BoneMatricesSetter() noexcept = default;

    virtual void draw(DrawPlanBuildInfo& buildInfo,
                      DrawableModificator** next,
                      size_t modifiactorsLeft,
                      Drawable& drawable) const override;

    std::vector<glm::mat4> matrices;
  };
}