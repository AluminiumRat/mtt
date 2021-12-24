#pragma once

#include <QtCore/QString>

#include <limits>
#include <memory>
#include <vector>

#include <mtt/Application/DrawModel/DrawModelAnimation.h>
#include <mtt/Application/DrawModel/DrawModelTransformTable.h>
#include <mtt/Application/DrawModel/DrawModelMeshNode.h>
#include <mtt/Render/SceneGraph/CombinedDrawableNode.h>
#include <mtt/Render/SceneGraph/Joint.h>

namespace mtt
{
  class DrawModel : public CombinedDrawableNode
  {
  public:
    DrawModel();
    DrawModel(const DrawModel&) = delete;
    DrawModel& operator = (const DrawModel&) = delete;
    virtual ~DrawModel() noexcept = default;

    inline DrawModelTransformTable& transformTable() noexcept;
    inline const DrawModelTransformTable& transformTable() const noexcept;

    inline size_t jointsNumber() const noexcept;
    inline Joint& getJoint(size_t index) noexcept;
    inline const Joint& getJoint(size_t index) const noexcept;
    /// bone index is index of transform from transform table, can be
    /// DrawModelTransformTable::notIndex
    void addJoint(std::unique_ptr<Joint> joint,
                  DrawModelTransformTable::Index boneIndex);
    /// Returns removed joint
    std::unique_ptr<Joint> removeJoint(const Joint& joint) noexcept;

    inline size_t meshNodeNumber() const noexcept;
    inline DrawModelMeshNode& getMeshNode(size_t index) noexcept;
    inline const DrawModelMeshNode& getMeshNode(size_t index) const noexcept;
    void addMeshNode(std::unique_ptr<DrawModelMeshNode> node);
    /// Returns removed node
    std::unique_ptr<DrawModelMeshNode> removeMeshNode(
                                        const DrawModelMeshNode& node) noexcept;

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
                                  const DrawModelAnimation& animation) noexcept;

    void updateFromAnimation( DrawModelAnimation& animation,
                              Application::TimeType time);

  private:
    struct JointRecord
    {
      std::unique_ptr<Joint> node;
      DrawModelTransformTable::Index boneIndex = 0;

      JointRecord() = default;
      JointRecord(const JointRecord&) = delete;
      JointRecord(JointRecord&&) = default;
      JointRecord& operator = (const JointRecord&) = delete;
      JointRecord& operator = (JointRecord&&) = default;
      ~JointRecord() noexcept = default;
    };
    using Joints = std::vector<JointRecord>;

    using Meshes = std::vector<std::unique_ptr<DrawModelMeshNode>>;

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
    DrawModelTransformTable _transformTable;
    Joints _joints;
    Meshes _meshes;
    Animations _animations;
  };

  inline DrawModelTransformTable& DrawModel::transformTable() noexcept
  {
    return _transformTable;
  }

  inline const DrawModelTransformTable&
                                      DrawModel::transformTable() const noexcept
  {
    return _transformTable;
  }

  inline size_t DrawModel::jointsNumber() const noexcept
  {
    return _joints.size();
  }

  inline Joint& DrawModel::getJoint(size_t index) noexcept
  {
    return *_joints[index].node;
  }

  inline const Joint& DrawModel::getJoint(size_t index) const noexcept
  {
    return *_joints[index].node;
  }

  inline size_t DrawModel::meshNodeNumber() const noexcept
  {
    return _meshes.size();
  }

  inline DrawModelMeshNode& DrawModel::getMeshNode(size_t index) noexcept
  {
    return *_meshes[index];
  }

  inline const DrawModelMeshNode& DrawModel::getMeshNode(
                                                    size_t index) const noexcept
  {
    return *_meshes[index];
  }

  inline size_t DrawModel::animationsNumber() const noexcept
  {
    return _animations.size();
  }
  
  inline const QString& DrawModel::animationName(
                                          size_t animationIndex) const noexcept
  {
    return _animations[animationIndex].name;
  }

  inline DrawModelAnimation& DrawModel::animation(
                                                size_t animationIndex) noexcept
  {
    return *_animations[animationIndex].animation;
  }

  inline const DrawModelAnimation& DrawModel::animation(
                                          size_t animationIndex) const noexcept
  {
    return *_animations[animationIndex].animation;
  }

  inline DrawModelAnimation* DrawModel::findAnimation(
                                                  const QString& name) noexcept
  {
    size_t index = _findAnimationIndex(name);
    if(index == std::numeric_limits<size_t>::max()) return nullptr;
    return &animation(index);
  }

  inline const DrawModelAnimation* DrawModel::findAnimation(
                                            const QString& name) const noexcept
  {
    size_t index = _findAnimationIndex(name);
    if (index == std::numeric_limits<size_t>::max()) return nullptr;
    return &animation(index);
  }

  inline size_t DrawModel::_findAnimationIndex(
                                    const QString& animationName) const noexcept
  {
    for (size_t index = 0; index < _animations.size(); index++)
    {
      if(_animations[index].name == animationName) return index;
    }
    return std::numeric_limits<size_t>::max();
  }
}