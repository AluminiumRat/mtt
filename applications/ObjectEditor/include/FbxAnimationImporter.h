#pragma once

#include <memory>
#include <set>

#include <mtt/fbx/BaseFbxImporter.h>

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
  virtual void processScene(FbxScene& scene) override;
  virtual void pushTranslation(FbxNode& node) override;

private:
  static void _fillTrack( AnimationTrack& track,
                          FbxNode& source,
                          const std::set<FbxTime>& times);

private:
  std::unique_ptr<AnimationObject> _result;
  FbxAnimLayer* _baseLayer;
};