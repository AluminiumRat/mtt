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

  virtual void visitConstAmbientLightObject(
                                const mtt::AmbientLightObject& object) override;
  virtual void visitConstAnimationObject(
                                  const mtt::AnimationObject& object) override;
  virtual void visitConstAnimationTrack(
                                    const mtt::AnimationTrack& object) override;
  virtual void visitConstBackgroundObject(
                                  const mtt::BackgroundObject& object) override;
  virtual void visitConstDirectLightObject(
                                const mtt::DirectLightObject& object) override;
  virtual void visitConstDisplayedObject(
                                  const mtt::DisplayedObject& object) override;
  virtual void visitConstEnvironmentModel(
                                  const mtt::EnvironmentModel& object) override;
  virtual void visitConstGeometryObject(const GeometryObject& object) override;
  virtual void visitConstLightObject(const mtt::LightObject& object) override;
  virtual void visitConstLODObject(const LODObject& object) override;
  virtual void visitConstMaterialObject(const MaterialObject& object) override;
  virtual void visitConstMeshObject(const MeshObject& object) override;
  virtual void visitConstMovableObject(
                                    const mtt::MovableObject& object) override;
  virtual void visitConstObject(const mtt::Object& object) override;
  virtual void visitConstRotatableObject(
                                  const mtt::RotatableObject& object) override;
  virtual void visitConstScalableObject(
                                  const mtt::ScalableObject& object) override;
  virtual void visitConstSkeletonObject(
                                    const mtt::SkeletonObject& object) override;

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
