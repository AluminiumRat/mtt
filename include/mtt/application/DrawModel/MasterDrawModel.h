#pragma once

#include <QtCore/QString>

#include <limits>
#include <memory>
#include <optional>
#include <vector>

#include <mtt/application/DrawModel/DrawModelAnimation.h>
#include <mtt/application/DrawModel/MeshControlNode.h>
#include <mtt/application/DrawModel/TransformTable.h>
#include <mtt/render/SceneGraph/CombinedDrawableNode.h>
#include <mtt/render/SceneGraph/Joint.h>
#include <mtt/utilities/Abort.h>

namespace mtt
{
  class MasterDrawModel : public CombinedDrawableNode
  {
  public:
    MasterDrawModel();
    MasterDrawModel(const MasterDrawModel&) = delete;
    MasterDrawModel& operator = (const MasterDrawModel&) = delete;
    virtual ~MasterDrawModel() noexcept = default;

    inline TransformTable& transformTable() noexcept;
    inline const TransformTable& transformTable() const noexcept;

    inline size_t jointsNumber() const noexcept;
    inline Joint& getJoint(size_t index) noexcept;
    inline const Joint& getJoint(size_t index) const noexcept;
    /// bone index is index of transform from transform table, can be
    /// TransformTable::notIndex
    inline TransformTable::Index boneIndex(size_t jointIndex) const noexcept;
    inline QString jointName(size_t jointIndex) const noexcept;
    /// Returns the index of the first joint with this name, or nullopt if
    /// no joint is found
    std::optional<size_t> findJoint(const QString& name) const noexcept;
    /// bone index is index of transform from transform table, can be
    /// TransformTable::notIndex
    void addJoint(std::unique_ptr<Joint> joint,
                  TransformTable::Index boneIndex,
                  const QString& name);
    /// Returns removed joint
    std::unique_ptr<Joint> removeJoint(Joint& joint) noexcept;

    inline size_t meshNodeNumber() const noexcept;
    inline MeshControlNode& getMeshNode(size_t index) noexcept;
    inline const MeshControlNode& getMeshNode(size_t index) const noexcept;
    void addMeshNode(std::unique_ptr<MeshControlNode> node);
    /// Returns removed node
    std::unique_ptr<MeshControlNode> removeMeshNode(
                                                MeshControlNode& node) noexcept;

    inline size_t animationsNumber() const noexcept;
    inline const QString& animationName(size_t animationIndex) const noexcept;
    inline DrawModelAnimation& animation(size_t animationIndex) noexcept;
    inline const DrawModelAnimation& animation(
                                          size_t animationIndex) const noexcept;
    inline DrawModelAnimation* findAnimation(const QString& name) noexcept;
    inline const DrawModelAnimation* findAnimation(
                                            const QString& name) const noexcept;
    void addAnimation(std::unique_ptr<DrawModelAnimation> animation,
                      const QString& animationName);
    /// Returns removed animation
    std::unique_ptr<DrawModelAnimation> removeAnimation(
                                        DrawModelAnimation& animation) noexcept;

    void updateFromAnimation( const DrawModelAnimation& animation,
                              Application::TimeType time);

    /// This is the protection of slave models. If you try to change the locked
    /// master model, the mtt::Abort will be called.
    /// The slave model locks the master model in the constructor.
    inline bool locked() const noexcept;
    void lock() noexcept;

  private:
    struct JointRecord
    {
      std::unique_ptr<Joint> node;
      TransformTable::Index boneIndex = 0;
      QString name;

      JointRecord() = default;
      JointRecord(const JointRecord&) = delete;
      JointRecord(JointRecord&&) = default;
      JointRecord& operator = (const JointRecord&) = delete;
      JointRecord& operator = (JointRecord&&) = default;
      ~JointRecord() noexcept = default;
    };
    using Joints = std::vector<JointRecord>;

    using Meshes = std::vector<std::unique_ptr<MeshControlNode>>;

    struct AnimationRecord
    {
      std::unique_ptr<DrawModelAnimation> animation;
      QString name;

      AnimationRecord() = default;
      AnimationRecord(const AnimationRecord&) = delete;
      AnimationRecord(AnimationRecord&&) = default;
      AnimationRecord& operator = (const AnimationRecord&) = delete;
      AnimationRecord& operator = (AnimationRecord&&) = default;
      ~AnimationRecord() noexcept = default;
    };
    using Animations = std::vector<AnimationRecord>;

  private:
    inline size_t _findAnimationIndex(
                                  const QString& animationName) const noexcept;

  private:
    TransformTable _transformTable;
    Joints _joints;
    Meshes _meshes;
    Animations _animations;
    bool _locked;
  };

  inline TransformTable& MasterDrawModel::transformTable() noexcept
  {
    return _transformTable;
  }

  inline const TransformTable& MasterDrawModel::transformTable() const noexcept
  {
    return _transformTable;
  }

  inline size_t MasterDrawModel::jointsNumber() const noexcept
  {
    return _joints.size();
  }

  inline Joint& MasterDrawModel::getJoint(size_t index) noexcept
  {
    return *_joints[index].node;
  }

  inline const Joint& MasterDrawModel::getJoint(size_t index) const noexcept
  {
    return *_joints[index].node;
  }

  inline TransformTable::Index MasterDrawModel::boneIndex(
                                              size_t jointIndex) const noexcept
  {
    return _joints[jointIndex].boneIndex;
  }

  inline QString MasterDrawModel::jointName(size_t jointIndex) const noexcept
  {
    return _joints[jointIndex].name;
  }

  inline size_t MasterDrawModel::meshNodeNumber() const noexcept
  {
    return _meshes.size();
  }

  inline MeshControlNode& MasterDrawModel::getMeshNode(size_t index) noexcept
  {
    return *_meshes[index];
  }

  inline const MeshControlNode& MasterDrawModel::getMeshNode(
                                                    size_t index) const noexcept
  {
    return *_meshes[index];
  }

  inline size_t MasterDrawModel::animationsNumber() const noexcept
  {
    return _animations.size();
  }
  
  inline const QString& MasterDrawModel::animationName(
                                          size_t animationIndex) const noexcept
  {
    return _animations[animationIndex].name;
  }

  inline DrawModelAnimation& MasterDrawModel::animation(
                                                size_t animationIndex) noexcept
  {
    if (_locked) mtt::Abort("MasterDrawModel::animation: model is locked");
    return *_animations[animationIndex].animation;
  }

  inline const DrawModelAnimation& MasterDrawModel::animation(
                                          size_t animationIndex) const noexcept
  {
    return *_animations[animationIndex].animation;
  }

  inline DrawModelAnimation* MasterDrawModel::findAnimation(
                                                  const QString& name) noexcept
  {
    if (_locked) mtt::Abort("MasterDrawModel::findAnimation: model is locked");
    size_t index = _findAnimationIndex(name);
    if(index == std::numeric_limits<size_t>::max()) return nullptr;
    return &animation(index);
  }

  inline const DrawModelAnimation* MasterDrawModel::findAnimation(
                                            const QString& name) const noexcept
  {
    size_t index = _findAnimationIndex(name);
    if (index == std::numeric_limits<size_t>::max()) return nullptr;
    return &animation(index);
  }

  inline size_t MasterDrawModel::_findAnimationIndex(
                                    const QString& animationName) const noexcept
  {
    for (size_t index = 0; index < _animations.size(); index++)
    {
      if(_animations[index].name == animationName) return index;
    }
    return std::numeric_limits<size_t>::max();
  }

  inline bool MasterDrawModel::locked() const noexcept
  {
    return _locked;
  }
}