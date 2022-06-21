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
    class CubeShadowmapProvider
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
      CubeShadowmapProvider(size_t framePoolsNumber,
                            glm::uvec2 frameExtent,
                            LogicalDevice& device);
      CubeShadowmapProvider(const CubeShadowmapProvider&) = delete;
      CubeShadowmapProvider& operator = (const CubeShadowmapProvider&) = delete;
      virtual ~CubeShadowmapProvider() noexcept = default;

      inline glm::uvec2 frameExtent() const noexcept;
      void setFrameExtent(glm::uvec2 frameExtent);

      inline AbstractField* targetField() const noexcept;
      void setTargetField(AbstractField* newField) noexcept;

      ImageView& getShadowMap(const CameraNode& frontCamera,
                              DrawPlanBuildInfo& buildInfo);

    private:
      struct ShadowmapRecord
      {
        std::unique_ptr<AbstractFrame> rightFrame;
        std::unique_ptr<AbstractFrame> leftFrame;
        std::unique_ptr<AbstractFrame> upFrame;
        std::unique_ptr<AbstractFrame> bottomFrame;
        std::unique_ptr<AbstractFrame> frontFrame;
        std::unique_ptr<AbstractFrame> backFrame;
        Ref<ImageView> samplerImageView;

        ShadowmapRecord() noexcept = default;
        ShadowmapRecord(ShadowmapRecord&& other) noexcept = default;
        ShadowmapRecord& operator = (ShadowmapRecord&& other) noexcept = default;
        ShadowmapRecord(const ShadowmapRecord&) = delete;
        ShadowmapRecord& operator = (const ShadowmapRecord&) = delete;
        ~ShadowmapRecord() = default;
      };

    private:
      ImageView* _tryUseExistingPlan(
                                  const std::vector<AbstractFramePlan*>& plans,
                                  const CameraNode& frontCamera,
                                  DrawPlan& drawPlan,
                                  const AbstractFramePlan& dependentFrame);
      ShadowmapRecord& _getOrCreateBlankShadowmap(size_t index);
      std::unique_ptr<AbstractFrame> _createFrame(Image& target, uint32_t face);
      void _buildNewMap(const CameraNode& frontCamera,
                        ShadowmapRecord& shadowmapRecord,
                        DrawPlan& drawPlan,
                        const AbstractFramePlan& dependentFrame,
                        ViewInfo& rootViewInfo);
      void _buildFramePlan( const CameraNode& renderCamera,
                            AbstractFrame& frame,
                            DrawPlan& drawPlan,
                            const AbstractFramePlan& dependentFrame,
                            ViewInfo& rootViewInfo);

    private:
      glm::ivec2 _frameExtent;
      ShadowmapBuilder _frameBuilder;

      using FramePool = std::vector<ShadowmapRecord>;
      using FramePools = std::vector<FramePool>;
      FramePools _framePools;
      size_t _currentPoolIndex;

      AbstractField* _targetField;
    };

    inline glm::uvec2 CubeShadowmapProvider::frameExtent() const noexcept
    {
      return _frameExtent;
    }

    inline AbstractField* CubeShadowmapProvider::targetField() const noexcept
    {
      return _targetField;
    }
  }
};
