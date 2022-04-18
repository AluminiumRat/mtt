#pragma once

#include <memory>
#include <set>

#include <mtt/editorLib/Objects/AnimationObject.h>
#include <mtt/editorLib/Objects/PositionAnimator.h>
#include <mtt/fbx/BaseFbxImporter.h>

namespace mtt
{
  class FbxAnimationImporter : public BaseFbxImporter
  {
  public:
    FbxAnimationImporter();
    FbxAnimationImporter(const FbxAnimationImporter&) = delete;
    FbxAnimationImporter& operator = (const FbxAnimationImporter&) = delete;
    virtual ~FbxAnimationImporter() noexcept = default;

    std::unique_ptr<AnimationObject> import(const char* filename);

  protected:
    virtual void processScene(FbxScene& scene) override;
    virtual void pushTranslation(FbxNode& node) override;

  private:
    static void _fillTrack( PositionAnimator& track,
                            FbxNode& source,
                            const std::set<FbxTime>& times);

  private:
    std::unique_ptr<AnimationObject> _result;
    FbxAnimLayer* _baseLayer;
  };
}