#pragma once

#include <mtt/dlPipeline/RenderPass/AmbientWeightPass.h>
#include <mtt/dlPipeline/RenderPass/BackgroundPass.h>
#include <mtt/dlPipeline/RenderPass/LightingPass.h>
#include <mtt/dlPipeline/RenderPass/PeelingGPass.h>
#include <mtt/dlPipeline/RenderPass/PeelingLayPass.h>
#include <mtt/dlPipeline/RenderPass/PeelingMergePass.h>

namespace mtt
{
  namespace dlPipeline
  {
    /// Implementation of depth peeling with deffered light
    class TransparencyPipeline
    {
    public:
      struct FrameData
      {
        Ref<FrameBuffer> gPassFrameBuffers[2];
        Ref<FrameBuffer> ambientWeightFrameBuffers[2];
        Ref<FrameBuffer> lightingFrameBuffer[2];
        Ref<FrameBuffer> backgroundFrameBuffer[2];
        Ref<FrameBuffer> mergeFrameBuffer;
        Ref<FrameBuffer> layFrameBuffer;

        Image* firstFrontBuffer;
        Image* accumulator;
      };

    public:
      TransparencyPipeline( PeelingGPass& gBufferPass,
                            AmbientWeightPass& ambientWeightPass,
                            LightingPass& lightingPass,
                            BackgroundPass& backgroundPass,
                            size_t stepsNumber);
      TransparencyPipeline(const TransparencyPipeline&) = delete;
      TransparencyPipeline& operator = (const TransparencyPipeline&) = delete;
      ~TransparencyPipeline() noexcept = default;
  
      FrameData createFrameData(ImageView& target,
                                ImageView& frameDepthMap);

      void scheduleDraw(FrameData& frameData,
                        AbstractFramePlan& plan,
                        DrawContext& drawContext);

    private:
      PeelingGPass& _gBufferPass;
      AmbientWeightPass& _ambientWeightPass;
      LightingPass& _lightingPass;
      BackgroundPass& _backgroundPass;
      Ref<PeelingMergePass> _mergePass;
      Ref<PeelingLayPass> _layPass;
      size_t _stepsNumber;
    };
  }
};
