#pragma once

#include <memory>
#include <vector>

#include <glm/vec2.hpp>

#include <mtt/clPipeline/Lighting/OpaqueShadowmapBuilder.h>
#include <mtt/clPipeline/Lighting/TransparentShadowmapBuilder.h>
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
      static constexpr VkFormat opaqueMapFormat = VK_FORMAT_R16_UNORM;

      /// R component is weighted average distance to transparent objects
      /// G component is weighted average square of distance to transparent
      /// objects
      /// B component is weight weight for G and B components
      /// If B component is 0 then no transparent objects rendered to this
      /// texel.
      static constexpr VkFormat transparentMapFormat =
                                                  VK_FORMAT_R16G16B16A16_UNORM;

      /// Primary layout of opaque map and transparent map
      static constexpr VkImageLayout shadowmapLayout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

      struct Shadowmaps
      {
        ImageView* opaqueMap;
        ImageView* transparentMap;
      };

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

      Shadowmaps getShadowMaps( const CameraNode& camera,
                                DrawPlanBuildInfo& buildInfo);

    private:
      struct FrameRecord
      {
        std::unique_ptr<AbstractFrame> opaqueFrame;
        Ref<ImageView> opaqueMapView;

        std::unique_ptr<AbstractFrame> transparentFrame;
        Ref<ImageView> transparentMapView;

        FrameRecord() noexcept = default;
        FrameRecord(FrameRecord&& other) noexcept = default;
        FrameRecord& operator = (FrameRecord&& other) noexcept = default;
        FrameRecord(const FrameRecord&) = delete;
        FrameRecord& operator = (const FrameRecord&) = delete;
        ~FrameRecord() = default;
      };

    private:
      FrameRecord* _tryUseExistingPlan(
                                  const std::vector<AbstractFramePlan*>& plans,
                                  const CameraNode& renderCamera,
                                  DrawPlan& drawPlan,
                                  const AbstractFramePlan& dependentFrame);
      void _buildNewMap(const CameraNode& renderCamera,
                        AbstractFrame& opaqueFrame,
                        AbstractFrame& transparentFrame,
                        DrawPlan& drawPlan,
                        const AbstractFramePlan& dependentFrame,
                        ViewInfo& rootViewInfo);
      FrameRecord& _getOrCreateFrame(size_t index);

    private:
      glm::uvec2 _frameExtent;
      OpaqueShadowmapBuilder _opaqueFrameBuilder;
      TransparentShadowmapBuilder _transparentFrameBuilder;

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
