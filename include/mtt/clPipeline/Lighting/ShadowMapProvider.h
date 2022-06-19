#pragma once

#include <memory>
#include <vector>

#include <glm/vec2.hpp>

#include <mtt/clPipeline/Lighting/ShadowmapBuilder.h>
#include <mtt/render/SceneGraph/CameraNode.h>

namespace mtt
{
  class AbstractField;
  struct DrawPlanBuildInfo;
  class ImageView;
  class LogicalDevice;

  namespace clPipeline
  {
    class ShadowMapProvider
    {
    public:
      /// R component is normalized linear distance to occluder
      /// G component is weighted average distance to transparent objects
      /// B component is weighted average square of distance to transparent
      /// objects
      /// A component is weight weight for G and B components
      /// If A component is 0 then no transparent objects rendered to this
      /// texel.
      static constexpr VkFormat shadowmapFormat = VK_FORMAT_R16G16B16A16_UNORM;
      static constexpr VkImageLayout shadowmapLayout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    public:
      ShadowMapProvider(size_t framePoolsNumber,
                        glm::uvec2 frameExtent,
                        LogicalDevice& device);
      ShadowMapProvider(const ShadowMapProvider&) = delete;
      ShadowMapProvider& operator = (const ShadowMapProvider&) = delete;
      virtual ~ShadowMapProvider() noexcept = default;

      inline glm::uvec2 frameExtent() const noexcept;
      void setFrameExtent(glm::uvec2 frameExtent);

      inline AbstractField* targetField() const noexcept;
      void setTargetField(AbstractField* newField) noexcept;

      ImageView& getShadowMap(const CameraNode& camera,
                              DrawPlanBuildInfo& buildInfo);

    private:
      struct FrameRecord
      {
        std::unique_ptr<AbstractFrame> frame;
        Ref<ImageView> samplerImageView;

        FrameRecord() noexcept = default;
        FrameRecord(FrameRecord&& other) noexcept = default;
        FrameRecord& operator = (FrameRecord&& other) noexcept = default;
        FrameRecord(const FrameRecord&) = delete;
        FrameRecord& operator = (const FrameRecord&) = delete;
        ~FrameRecord() = default;
      };

    private:
      ImageView* _tryUseExistingPlan(
                                  const std::vector<AbstractFramePlan*>& plans,
                                  const CameraNode& renderCamera,
                                  DrawPlan& drawPlan,
                                  const AbstractFramePlan& dependentFrame);
      void _buildNewMap(const CameraNode& renderCamera,
                        AbstractFrame& frame,
                        DrawPlan& drawPlan,
                        const AbstractFramePlan& dependentFrame,
                        ViewInfo& rootViewInfo);
      FrameRecord& _getOrCreateFrame(size_t index);

    private:
      glm::ivec2 _frameExtent;
      ShadowmapBuilder _frameBuilder;

      using FramePool = std::vector<FrameRecord>;
      using FramePools = std::vector<FramePool>;
      FramePools _framePools;
      size_t _currentPoolIndex;

      AbstractField* _targetField;
    };

    inline glm::uvec2 ShadowMapProvider::frameExtent() const noexcept
    {
      return _frameExtent;
    }

    inline AbstractField* ShadowMapProvider::targetField() const noexcept
    {
      return _targetField;
    }
  }
};
