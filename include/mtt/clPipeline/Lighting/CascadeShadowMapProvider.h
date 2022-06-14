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

      CascadeInfo getShadowMap( size_t cascadeSize,
                                const CameraNode& topViewCamera,
                                DrawPlanBuildInfo& buildInfo);

    private:
      /// Area of shadow map in clip space
      struct Area
      {
        glm::vec2 topleftCorner;
        glm::vec2 size;
      };

    private:
      void _setupRenderCamera(const CameraNode& topViewCamera,
                              CameraNode& renderCamera,
                              const Area& mapPart) const noexcept;

      Area _getTopArea( const CameraNode& topViewCamera,
                        DrawPlanBuildInfo& buildInfo) const noexcept;
      glm::vec2 _getCascadeDirectionPoint(const CameraNode& topViewCamera,
                                          DrawPlanBuildInfo& buildInfo,
                                          glm::vec2 startPoint) const noexcept;
      Area _alignArea(const Area& source) const noexcept;
    };
  }
};
