# pragma once

#include <QtCore/QDir>
#include <QtCore/QString>

#include <Objects/OEVisitor.h>

namespace mtt
{
  class DataStream;
}

class ObjectSaver : private OEVisitor
{
public:
  static void saveObject( const mtt::Object& object,
                          mtt::DataStream& stream,
                          const QDir& fileDirectory);

private:
  struct ObjectIdSaver;

private:
  ObjectSaver(mtt::DataStream& stream, const QDir& fileDirectory);
  ObjectSaver(const ObjectSaver&) = delete;
  ObjectSaver& operator = (const ObjectSaver&) = delete;
  virtual ~ObjectSaver() noexcept = default;

  virtual void visit(const mtt::AmbientLightObject& object) override;
  virtual void visit(const mtt::AnimationObject& object) override;
  virtual void visit(const mtt::AnimationTrack& object) override;
  virtual void visit(const mtt::BackgroundObject& object) override;
  virtual void visit(const mtt::DirectLightObject& object) override;
  virtual void visit(const mtt::DisplayedObject& object) override;
  virtual void visit(const mtt::EnvironmentModel& object) override;
  virtual void visit(const GeometryObject& object) override;
  virtual void visit(const mtt::LightObject& object) override;
  virtual void visit(const LODObject& object) override;
  virtual void visit(const MaterialObject& object) override;
  virtual void visit(const MeshObject& object) override;
  virtual void visit(const mtt::MovableObject& object) override;
  virtual void visit(const mtt::Object& object) override;
  virtual void visit(const mtt::RotatableObject& object) override;
  virtual void visit(const mtt::ScalableObject& object) override;
  virtual void visit(const mtt::SkeletonObject& object) override;

  void _saveGeometry(const mtt::CommonMeshGeometry& geometry);
  void _saveBoneRefs(const BoneRefBatch& refs);
  void _writeFilename(const QString& filename);
  template<typename ValueType>
  void _writeKeypoint(mtt::ValueKeypoint<ValueType,
                      mtt::AnimationTrack::TimeType> keypoint);
  void _writeCubemapData(const mtt::CubemapObject& object);

private:
  mtt::DataStream& _stream;
  QDir _fileDirectory;
};
