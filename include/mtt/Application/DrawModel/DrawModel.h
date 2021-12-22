#pragma once

#include <memory>
#include <vector>

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

  private:
    DrawModelTransformTable _transformTable;
    Joints _joints;
    Meshes _meshes;
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
}