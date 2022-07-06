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
    class CubeShadowmapProvider
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

      static constexpr VkImageLayout shadowmapLayout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

      struct Shadowmaps
      {
        ImageView* opaqueMap;
        ImageView* transparentMap;
      };

    public:
      CubeShadowmapProvider(size_t framePoolsNumber,
                            uint32_t frameExtent,
                            LogicalDevice& device);
      CubeShadowmapProvider(const CubeShadowmapProvider&) = delete;
      CubeShadowmapProvider& operator = (const CubeShadowmapProvider&) = delete;
      virtual ~CubeShadowmapProvider() noexcept = default;

      inline uint32_t frameExtent() const noexcept;
      void setFrameExtent(uint32_t frameExtent);

      inline AbstractField* targetField() const noexcept;
      void setTargetField(AbstractField* newField) noexcept;

      Shadowmaps getShadowMaps( const CameraNode& frontCamera,
                                DrawPlanBuildInfo& buildInfo);

    private:
      struct ShadowmapRecord
      {
        std::unique_ptr<AbstractFrame> rightOpaqueFrame;
        std::unique_ptr<AbstractFrame> rightTransparentFrame;
        std::unique_ptr<AbstractFrame> leftOpaqueFrame;
        std::unique_ptr<AbstractFrame> leftTransparentFrame;
        std::unique_ptr<AbstractFrame> upOpaqueFrame;
        std::unique_ptr<AbstractFrame> upTransparentFrame;
        std::unique_ptr<AbstractFrame> bottomOpaqueFrame;
        std::unique_ptr<AbstractFrame> bottomTransparentFrame;
        std::unique_ptr<AbstractFrame> frontOpaqueFrame;
        std::unique_ptr<AbstractFrame> frontTransparentFrame;
        std::unique_ptr<AbstractFrame> backOpaqueFrame;
        std::unique_ptr<AbstractFrame> backTransparentFrame;
        Ref<ImageView> opaqueMapView;
        Ref<ImageView> transparentMapView;

        ShadowmapRecord() noexcept = default;
        ShadowmapRecord(ShadowmapRecord&& other) noexcept = default;
        ShadowmapRecord& operator = (ShadowmapRecord&& other) noexcept = default;
        ShadowmapRecord(const ShadowmapRecord&) = delete;
        ShadowmapRecord& operator = (const ShadowmapRecord&) = delete;
        ~ShadowmapRecord() = default;
      };

    private:
      ShadowmapRecord* _tryUseExistingPlan(
                                  const std::vector<AbstractFramePlan*>& plans,
                                  const CameraNode& frontCamera,
                                  DrawPlan& drawPlan,
                                  const AbstractFramePlan& dependentFrame);
      ShadowmapRecord& _getOrCreateBlankShadowmap(size_t index);

      template<typename Builder>
      std::unique_ptr<AbstractFrame> _createFrame(Builder& builder,
                                                  Image& shadowMap,
                                                  uint32_t face);

      void _buildNewMap(const CameraNode& frontCamera,
                        ShadowmapRecord& shadowmapRecord,
                        DrawPlan& drawPlan,
                        const AbstractFramePlan& dependentFrame,
                        ViewInfo& rootViewInfo);
      void _buildFramePlan( const CameraNode& renderCamera,
                            AbstractFrame& opaqueFrame,
                            AbstractFrame& transparentFrame,
                            DrawPlan& drawPlan,
                            const AbstractFramePlan& dependentFrame,
                            ViewInfo& rootViewInfo);

    private:
      uint32_t _frameExtent;
      OpaqueShadowmapBuilder _opaqueFrameBuilder;
      TransparentShadowmapBuilder _transparentFrameBuilder;

      using FramePool = std::vector<ShadowmapRecord>;
      using FramePools = std::vector<FramePool>;
      FramePools _framePools;
      size_t _currentPoolIndex;

      AbstractField* _targetField;
    };

    inline uint32_t CubeShadowmapProvider::frameExtent() const noexcept
    {
      return _frameExtent;
    }

    inline AbstractField* CubeShadowmapProvider::targetField() const noexcept
    {
      return _targetField;
    }
  }
};
