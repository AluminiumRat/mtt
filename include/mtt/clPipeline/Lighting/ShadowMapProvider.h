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
      static constexpr VkFormat shadowmapFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
      static constexpr VkImageLayout shadowmapLayout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    public:
      struct ShadowMapInfo
      {
        ImageView* map;
        struct
        {
          float multiplicator;
          glm::vec2 shift;
        } coordCorrection; //resultCoord = originCoord * multiplicator + shift
      };
      using CascadeInfo = std::vector<ShadowMapInfo>;

    public:
      ShadowMapProvider(size_t framePoolsNumber,
                        glm::uvec2 frameExtent,
                        LogicalDevice& device);
      ShadowMapProvider(const ShadowMapProvider&) = delete;
      ShadowMapProvider& operator = (const ShadowMapProvider&) = delete;
      virtual ~ShadowMapProvider() noexcept = default;

      inline glm::uvec2 frameExtent() const noexcept;
      void setFrameExtent(glm::uvec2 frameExtent);

      inline CameraNode& camera() noexcept;
      inline const CameraNode& camera() const noexcept;

      inline AbstractField* targetField() const noexcept;
      void setTargetField(AbstractField* newField) noexcept;

      CascadeInfo createShadowMap(size_t cascadeSize,
                                  DrawPlanBuildInfo& buildInfo);

    private:
      struct FrameRecord
      {
        std::unique_ptr<OneTargetFrameBuilder::Frame> frame;
        Ref<ImageView> samplerImageView;

        FrameRecord() noexcept = default;
        FrameRecord(FrameRecord&& other) noexcept = default;
        FrameRecord& operator = (FrameRecord&& other) noexcept = default;
        FrameRecord(const FrameRecord&) = delete;
        FrameRecord& operator = (const FrameRecord&) = delete;
        ~FrameRecord() = default;
      };

      /// Area of shadow map in clip space
      struct Area
      {
        glm::vec2 topleftCorner;
        glm::vec2 size;
      };

    private:
      ImageView& _getShadowMap( const Area& mapPart,
                                DrawPlan& drawPlan,
                                const AbstractFramePlan& dependentFrame,
                                ViewInfo& rootViewInfo);
      ImageView* _tryUseExistingPlan(
                                  const std::vector<AbstractFramePlan*>& plans,
                                  CameraNode& renderCamera,
                                  DrawPlan& drawPlan,
                                  const AbstractFramePlan& dependentFrame);
      void _buildNewMap(CameraNode& renderCamera,
                        OneTargetFrameBuilder::Frame& frame,
                        DrawPlan& drawPlan,
                        const AbstractFramePlan& dependentFrame,
                        ViewInfo& rootViewInfo);
      FrameRecord& _getOrCreateFrame(size_t index);
      void _setupRenderCamera(CameraNode& renderCamera,
                              const Area& mapPart) const noexcept;

      Area _getTopArea(DrawPlanBuildInfo& buildInfo) const noexcept;
      glm::vec2 _getCascadeDirectionPoint(DrawPlanBuildInfo& buildInfo,
                                          glm::vec2 startPoint) const noexcept;
      Area _alignArea(const ShadowMapProvider::Area& source) const noexcept;

    private:
      glm::ivec2 _frameExtent;
      ShadowmapBuilder _frameBuilder;

      using FramePool = std::vector<FrameRecord>;
      using FramePools = std::vector<FramePool>;
      FramePools _framePools;
      size_t _currentPoolIndex;

      CameraNode _camera;
      AbstractField* _targetField;
    };

    inline glm::uvec2 ShadowMapProvider::frameExtent() const noexcept
    {
      return _frameExtent;
    }

    inline CameraNode& ShadowMapProvider::camera() noexcept
    {
      return _camera;
    }

    inline const CameraNode& ShadowMapProvider::camera() const noexcept
    {
      return _camera;
    }

    inline AbstractField* ShadowMapProvider::targetField() const noexcept
    {
      return _targetField;
    }
  }
};
