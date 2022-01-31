#pragma once

#include <mtt/render/DrawPlan/StageIndex.h>
#include <mtt/render/SceneRenderer/FrameType.h>

namespace mtt
{
  namespace clPipeline
  {
    constexpr FrameType colorFrameType = baseFrameTypes::colorFrameType;
    constexpr FrameType uidFrameType = baseFrameTypes::uidFrameType;
    constexpr FrameType depthmapFrameType = baseFrameTypes::depthmapFrameType;
    constexpr FrameType shadowmapFrameType(3);

    constexpr StageIndex subsidiaryStage = baseStages::subsidiaryStage;

    /// Stages for color render
    constexpr StageIndex modificatorsPrepareStage =
                                          baseStages::modificatorsPrepareStage;
    constexpr StageIndex gBufferStage(1);
    constexpr StageIndex ambientWeightStage(2);
    constexpr StageIndex lightingStage(3);
    constexpr StageIndex emissionStage(4);
    constexpr StageIndex backgroundStage(5);
    constexpr StageIndex forwardLightStage(6);
    constexpr StageIndex selectionStage(7);

    /// Single stage for shadowmap render
    constexpr StageIndex shadowmapStage(0);

    /// Single stage for uid render
    constexpr StageIndex uidStage = baseStages::uidStage;
  
    /// Single stage for depthmap render
    constexpr StageIndex depthmapStage = baseStages::depthmapStage;
  }
}