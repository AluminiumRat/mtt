#pragma once

#include <memory>
#include <set>

#include <mtt/editorLib/Objects/AnimationObject.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/fbx/BaseFbxImporter.h>

class FbxAnimationImporter : public mtt::BaseFbxImporter
{
public:
  FbxAnimationImporter();
  FbxAnimationImporter(const FbxAnimationImporter&) = delete;
  FbxAnimationImporter& operator = (const FbxAnimationImporter&) = delete;
  virtual ~FbxAnimationImporter() noexcept = default;

  std::unique_ptr<mtt::AnimationObject> import(const char* filename);

protected:
  virtual void processScene(FbxScene& scene) override;
  virtual void pushTranslation(FbxNode& node) override;

private:
  static void _fillTrack( mtt::AnimationTrack& track,
                          FbxNode& source,
                          const std::set<FbxTime>& times);

private:
  std::unique_ptr<mtt::AnimationObject> _result;
  FbxAnimLayer* _baseLayer;
};