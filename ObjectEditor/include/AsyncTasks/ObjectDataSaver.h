# pragma once

#include <QtCore/QDir>
#include <QtCore/QString>

#include <Objects/OEVisitor.h>

namespace mtt
{
  class DataStream;
}

class ObjectDataSaver : private OEVisitor
{
public:
  static void saveObject( const mtt::Object& object,
                          mtt::DataStream& stream,
                          const QDir& fileDirectory);

private:
  struct ObjectIdSaver;

private:
  ObjectDataSaver(mtt::DataStream& stream, const QDir& fileDirectory);
  ObjectDataSaver(const ObjectDataSaver&) = delete;
  ObjectDataSaver& operator = (const ObjectDataSaver&) = delete;
  virtual ~ObjectDataSaver() noexcept = default;

  virtual void visit(const AnimationObject& object) override;
  virtual void visit(const AnimationTrack& object) override;
  virtual void visit(const DisplayedObject& object) override;
  virtual void visit(const GeometryObject& object) override;
  virtual void visit(const LODObject& object) override;
  virtual void visit(const MaterialObject& object) override;
  virtual void visit(const MeshObject& object) override;
  virtual void visit(const MovableObject& object) override;
  virtual void visit(const mtt::Object& object) override;
  virtual void visit(const RotatableObject& object) override;
  virtual void visit(const ScalableObject& object) override;
  virtual void visit(const SkeletonObject& object) override;

  void saveGeometry(const mtt::CommonMeshGeometry& geometry);
  void saveBoneRefs(const BoneRefBatch& refs);
  void writeFilename(const QString& filename);
  template<typename ValueType>
  void writeKeypoint(
              mtt::ValueKeypoint<ValueType, AnimationTrack::TimeType> keypoint);

private:
  mtt::DataStream& _stream;
  QDir _fileDirectory;
};
