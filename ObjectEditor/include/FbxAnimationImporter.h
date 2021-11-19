#pragma once

#include <memory>
#include <set>

#include <mtt/Fbx/BaseFbxImporter.h>

#include <Objects/AnimationObject.h>
#include <Objects/AnimationTrack.h>

class FbxAnimationImporter : public mtt::BaseFbxImporter
{
public:
  FbxAnimationImporter();
  FbxAnimationImporter(const FbxAnimationImporter&) = delete;
  FbxAnimationImporter& operator = (const FbxAnimationImporter&) = delete;
  virtual ~FbxAnimationImporter() noexcept = default;

  std::unique_ptr<AnimationObject> import(const char* filename);

protected:
  virtual void processScene(FbxScene& scene);
  virtual void pushTranslation(FbxNode& node) override;

private:
  using TimeType = AnimationTrack::TimeType;

  std::set<TimeType> _getKeypointTimes(FbxNode& node);
  static void _fillKeypointTimes( std::set<TimeType>& timeSet,
                                  FbxAnimCurve& curve);
  static void _fillTrack( AnimationTrack& track,
                          FbxNode& source,
                          const std::set<TimeType>& times);

private:
  std::unique_ptr<AnimationObject> _result;
  FbxAnimLayer* _baseLayer;
};