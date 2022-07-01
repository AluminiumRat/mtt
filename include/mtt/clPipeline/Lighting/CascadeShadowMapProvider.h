#pragma once

#include <memory>
#include <vector>

#include <glm/vec2.hpp>

#include <mtt/clPipeline/Lighting/ShadowMapProvider.h>
#include <mtt/render/SceneGraph/CameraNode.h>

namespace mtt
{
  class AbstractField;
  struct DrawPlanBuildInfo;
  class ImageView;
  class LogicalDevice;

  namespace clPipeline
  {
    class CascadeShadowMapProvider : public ShadowMapProvider
    {
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
      CascadeShadowMapProvider( size_t framePoolsNumber,
                                glm::uvec2 frameExtent,
                                LogicalDevice& device);
      CascadeShadowMapProvider(const CascadeShadowMapProvider&) = delete;
      CascadeShadowMapProvider& operator = (
                                      const CascadeShadowMapProvider&) = delete;
      virtual ~CascadeShadowMapProvider() noexcept = default;

      CascadeInfo getShadowMap( const CameraNode& shadowmapCamera,
                                const CameraNode& viewCamera,
                                size_t cascadeSize,
                                DrawPlanBuildInfo& buildInfo);

    private:
      /// Area of shadow map in clip space
      struct Area
      {
        glm::vec2 topleftCorner;
        glm::vec2 size;
      };

    private:
      void _setupRenderCamera(const CameraNode& shadowmapCamera,
                              CameraNode& renderCamera,
                              const Area& mapPart) const noexcept;

      Area _getTopArea( const CameraNode& shadowmapCamera,
                        const CameraNode& viewCamera) const noexcept;

      glm::vec2 _getCascadeDirectionPoint(const CameraNode& shadowmapCamera,
                                          const CameraNode& viewCamera,
                                          glm::vec2 startPoint) const noexcept;

      Area _alignArea(const Area& source) const noexcept;
    };
  }
};
