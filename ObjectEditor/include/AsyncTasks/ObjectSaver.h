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

  virtual void visit(const AmbientLightObject& object) override;
  virtual void visit(const AnimationObject& object) override;
  virtual void visit(const AnimationTrack& object) override;
  virtual void visit(const BackgroundObject& object) override;
  virtual void visit(const DirectLightObject& object) override;
  virtual void visit(const DisplayedObject& object) override;
  virtual void visit(const GeometryObject& object) override;
  virtual void visit(const LightObject& object) override;
  virtual void visit(const LODObject& object) override;
  virtual void visit(const MaterialObject& object) override;
  virtual void visit(const MeshObject& object) override;
  virtual void visit(const MovableObject& object) override;
  virtual void visit(const mtt::Object& object) override;
  virtual void visit(const RotatableObject& object) override;
  virtual void visit(const ScalableObject& object) override;
  virtual void visit(const SkeletonObject& object) override;

  void _saveGeometry(const mtt::CommonMeshGeometry& geometry);
  void _saveBoneRefs(const BoneRefBatch& refs);
  void _writeFilename(const QString& filename);
  template<typename ValueType>
  void _writeKeypoint(
              mtt::ValueKeypoint<ValueType, AnimationTrack::TimeType> keypoint);
  void _writeCubemapData(const CubemapObject& object);

private:
  mtt::DataStream& _stream;
  QDir _fileDirectory;
};
