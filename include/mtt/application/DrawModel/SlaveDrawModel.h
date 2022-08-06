#pragma once

#include <map>
#include <memory>

#include <mtt/application/DrawModel/MasterDrawModel.h>
#include <mtt/application/DrawModel/TransformTable.h>
#include <mtt/application/DrawModel/SkinControlNode.h>
#include <mtt/render/SceneGraph/CombinedDrawableNode.h>
#include <mtt/render/SceneGraph/Joint.h>

namespace mtt
{
  class SlaveDrawModel : public CombinedDrawableNode
  {
  public:
    explicit SlaveDrawModel(std::shared_ptr<MasterDrawModel> masterModel);
    SlaveDrawModel(const SlaveDrawModel&) = delete;
    SlaveDrawModel& operator = (const SlaveDrawModel&) = delete;
    virtual ~SlaveDrawModel() noexcept = default;

    inline Joint* findJoint(const QString& name) noexcept;
    inline const Joint* findJoint(const QString& name) const noexcept;

    inline size_t animationsNumber() const noexcept;
    inline const QString& animationName(size_t animationIndex) const noexcept;
    inline const DrawModelAnimation& animation(
                                          size_t animationIndex) const noexcept;
    inline const DrawModelAnimation* findAnimation(
                                            const QString& name) const noexcept;

    void updateFromAnimation( const DrawModelAnimation& animation, TimeT time);

  private:
    struct JointRecord
    {
      std::unique_ptr<Joint> node;
      TransformTable::Index boneIndex = 0;

      JointRecord() = default;
      JointRecord(const JointRecord&) = delete;
      JointRecord(JointRecord&&) = default;
      JointRecord& operator = (const JointRecord&) = delete;
      JointRecord& operator = (JointRecord&&) = default;
      ~JointRecord() noexcept = default;
    };
    using Joints = std::vector<JointRecord>;

    using Meshes = std::vector<std::unique_ptr<SkinControlNode>>;

    using NodeMap = std::map<const AbstractNode*, AbstractNode*>;

  private:
    void _copyJoints(NodeMap& nodeMap);
    void _copyMeshes(NodeMap& nodeMap);
    void _connectNodes(const NodeMap& nodeMap);
    void _switchBoneRefs(const NodeMap& nodeMap);

  private:
    std::shared_ptr<MasterDrawModel> _masterModel;

    TransformTable _transformTable;
    Joints _joints;
    Meshes _meshes;
  };

  inline Joint* SlaveDrawModel::findJoint(const QString& name) noexcept
  {
    const MasterDrawModel& masterModel = *_masterModel;
    std::optional<size_t> jointIndex = masterModel.findJoint(name);
    if(!jointIndex.has_value()) return nullptr;
    return _joints[jointIndex.value()].node.get();
  }

  inline const Joint* SlaveDrawModel::findJoint(
                                            const QString& name) const noexcept
  {
    const MasterDrawModel& masterModel = *_masterModel;
    std::optional<size_t> jointIndex = masterModel.findJoint(name);
    if (!jointIndex.has_value()) return nullptr;
    return _joints[jointIndex.value()].node.get();
  }

  inline size_t SlaveDrawModel::animationsNumber() const noexcept
  {
    const MasterDrawModel& masterModel = *_masterModel;
    return masterModel.animationsNumber();
  }
  
  inline const QString& SlaveDrawModel::animationName(
                                          size_t animationIndex) const noexcept
  {
    const MasterDrawModel& masterModel = *_masterModel;
    return masterModel.animationName(animationIndex);
  }
  
  inline const DrawModelAnimation& SlaveDrawModel::animation(
                                          size_t animationIndex) const noexcept
  {
    const MasterDrawModel& masterModel = *_masterModel;
    return masterModel.animation(animationIndex);
  }
  
  inline const DrawModelAnimation* SlaveDrawModel::findAnimation(
                                            const QString& name) const noexcept
  {
    const MasterDrawModel& masterModel = *_masterModel;
    return masterModel.findAnimation(name);
  }
}