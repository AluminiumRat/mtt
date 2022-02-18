#include <chrono>
#include <stdexcept>

#include <QtCore/QFileInfo>

#include <mtt/editorLib/FbxAnimationImporter.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

FbxAnimationImporter::FbxAnimationImporter() :
  _baseLayer(nullptr)
{
}

std::unique_ptr<mtt::AnimationObject> FbxAnimationImporter::import(
                                                          const char* filename)
{
  _baseLayer = nullptr;

  QFileInfo file(filename);
  _result.reset(new mtt::AnimationObject(file.baseName(), true));
  startImporting(filename);

  return std::move(_result);
}

void FbxAnimationImporter::processScene(FbxScene& scene)
{
  FbxAnimStack* animStack = scene.GetCurrentAnimationStack();
  if(animStack == nullptr) throw std::runtime_error("No animation stacks in the file.");

  int numAnimLayers = animStack->GetMemberCount(
                                FbxCriteria::ObjectType(FbxAnimLayer::ClassId));
  if(numAnimLayers < 1) throw std::runtime_error("No animation layers in the file.");
  _baseLayer = static_cast<FbxAnimLayer*>(animStack->GetMember(
                                FbxCriteria::ObjectType(FbxAnimLayer::ClassId),
                                0));
  if(_baseLayer == nullptr) throw std::runtime_error("Unable to get animation layer from file.");
}

void FbxAnimationImporter::pushTranslation(FbxNode& node)
{
  if(_baseLayer == nullptr) mtt::Abort("FbxAnimationImporter::pushTranslation: base layer is null.");

  std::set<FbxTime> timeSet = getKeypointTimes(node, *_baseLayer);
  if(!timeSet.empty())
  {
    std::unique_ptr<mtt::AnimationTrack> track(
                                new mtt::AnimationTrack(node.GetName(), true));
    _fillTrack(*track, node, timeSet);
    _result->addChild(std::move(track));
  }

  BaseFbxImporter::pushTranslation(node);
}

void FbxAnimationImporter::_fillTrack(mtt::AnimationTrack& track,
                                      FbxNode& source,
                                      const std::set<FbxTime>& times)
{
  using TimeType = mtt::AnimationTrack::TimeType;

  for(const FbxTime& fbxTime : times)
  {
    using MediumTime = std::chrono::duration<FbxLongLong, std::ratio<1, 1000>>;
    MediumTime medium = MediumTime(fbxTime.GetMilliSeconds());
    TimeType time = std::chrono::duration_cast<TimeType>(medium);

    FbxDouble3 fbxPosition = source.LclTranslation.EvaluateValue(fbxTime);
    glm::vec3 position(fbxPosition[0], fbxPosition[1], fbxPosition[2]);
    track.addPositionKeypoint(
                std::make_unique<mtt::AnimationTrack::PositionKeypoint>(
                                                    position,
                                                    time,
                                                    mtt::LINEAR_INTERPOLATION));

    FbxDouble3 fbxRotation = source.LclRotation.EvaluateValue(fbxTime);
    glm::quat rotation(glm::vec3( glm::radians(fbxRotation[0]),
                                  glm::radians(fbxRotation[1]),
                                  glm::radians(fbxRotation[2])));
    track.addRotationKeypoint(
                std::make_unique<mtt::AnimationTrack::RotationKeypoint>(
                                                    rotation,
                                                    time,
                                                    mtt::LINEAR_INTERPOLATION));

    FbxDouble3 fbxScale = source.LclScaling.EvaluateValue(fbxTime);
    glm::vec3 scale(fbxScale[0], fbxScale[1], fbxScale[2]);
    track.addScaleKeypoint(
                std::make_unique<mtt::AnimationTrack::ScaleKeypoint>(
                                                    scale,
                                                    time,
                                                    mtt::LINEAR_INTERPOLATION));
  }
}
