#pragma once

#include <memory>
#include <vector>

#include <glm/vec2.hpp>

#include <mtt/clPipeline/Lighting/ShadowmapBuilder.h>
#include <mtt/render/SceneGraph/CameraNode.h>

namespace mtt
{
  class LogicalDevice;
  class AbstractField;

  namespace clPipeline
  {
    class ShadowMapProvider
    {
    public:
      /// R component is normalized linear distance to occluder
      static constexpr VkFormat shadowmapFormat = VK_FORMAT_R32_SFLOAT;
      static constexpr VkImageLayout shadowmapLayout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    public:
      /// Area of shadow map in clip space
      struct Area
      {
        glm::vec2 topleftCorner;
        glm::vec2 size;
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

      inline CameraNode& camera() noexcept;
      inline const CameraNode& camera() const noexcept;

      inline AbstractField* targetField() const noexcept;
      void setTargetField(AbstractField* newField) noexcept;

      ImageView& createShadowMap( const Area& mapPart,
                                  DrawPlan& drawPlan,
                                  const AbstractFramePlan& dependentFrame,
                                  ViewInfo& rootViewInfo);
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

      FrameRecord& _getOrCreateFrame(size_t index);
      void _setupRenderCamera(CameraNode& renderCamera,
                              const Area& mapPart) const noexcept;

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
