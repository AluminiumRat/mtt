#pragma once

#include <mtt/Render/DrawPlan/StageIndex.h>
#include <mtt/Render/SceneRenderer/FrameType.h>

namespace mtt
{
  namespace DLPipeline
  {
    constexpr FrameType colorFrameType = baseFrameTypes::colorFrameType;
    constexpr FrameType uidFrameType = baseFrameTypes::uidFrameType;
    constexpr FrameType depthmapFrameType = baseFrameTypes::depthmapFrameType;
    constexpr FrameType shadowmapFrameType(3);

    constexpr StageIndex subsidiaryStage = baseStages::subsidiaryStage;

    /// Stages for color render
    constexpr StageIndex gBufferStage(0);
    constexpr StageIndex ambientWeightStage(1);
    constexpr StageIndex lightingStage(2);
    constexpr StageIndex emissionStage(3);
    constexpr StageIndex backgroundStage(4);
    constexpr StageIndex peelingGBufferStage(5);
    constexpr StageIndex peelingAmbientWeightStage(6);
    constexpr StageIndex peelingLightingStage(7);
    constexpr StageIndex peelingBackgroundStage(8);
    constexpr StageIndex instrumentStage(9);
    constexpr StageIndex selectionStage(10);
  
    /// Single stage for shadowmap render
    constexpr StageIndex shadowmapStage(0);

    /// Single stage for uid render
    constexpr StageIndex uidStage = baseStages::uidStage;
  
    /// Single stage for depthmap render
    constexpr StageIndex depthmapStage = baseStages::depthmapStage;
  }
}