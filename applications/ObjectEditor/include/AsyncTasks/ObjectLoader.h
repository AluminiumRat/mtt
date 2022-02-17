#pragma once

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/application/DataStream.h>

#include <AsyncTasks/ObjectBuilder.h>
#include <Objects/OEVisitor.h>

class ObjectLoader : private OEVisitor
{
public:
  template<typename ObjectType>
  inline static std::unique_ptr<ObjectType> loadObject(
                                              bool canBeRenamed,
                                              mtt::DataStream& stream,
                                              QDir& fileDirectory,
                                              mtt::UID::ValueType mixUIDValue);

private:
  ObjectLoader( mtt::DataStream& stream,
                const QDir& fileDirectory,
                mtt::UID::ValueType mixUIDValue);
  ObjectLoader(const ObjectLoader&) = delete;
  ObjectLoader& operator = (const ObjectLoader&) = delete;
  virtual ~ObjectLoader() noexcept = default;

  virtual void visitAmbientLightObject(
                                      mtt::AmbientLightObject& object) override;
  virtual void visitAnimationObject(mtt::AnimationObject& object) override;
  virtual void visitAnimationTrack(mtt::AnimationTrack& object) override;
  virtual void visitBackgroundObject(mtt::BackgroundObject& object) override;
  virtual void visitDirectLightObject(mtt::DirectLightObject& object) override;
  virtual void visitDisplayedObject(mtt::DisplayedObject& object) override;
  virtual void visitEnvironmentModel(mtt::EnvironmentModel& object) override;
  virtual void visitGeometryObject(GeometryObject& object) override;
  virtual void visitLightObject(mtt::LightObject& object) override;
  virtual void visitLODObject(LODObject& object) override;
  virtual void visitMaterialObject(MaterialObject& object) override;
  virtual void visitMeshObject(MeshObject& object) override;
  virtual void visitMovableObject(mtt::MovableObject& object) override;
  virtual void visitRotatableObject(mtt::RotatableObject& object) override;
  virtual void visitScalableObject(mtt::ScalableObject& object) override;
  virtual void visitSkeletonObject(mtt::SkeletonObject& object) override;

  QString _loadFilename();
  std::unique_ptr<BoneRefBatch> readBoneRefs();
  void _readGeometry(mtt::CommonMeshGeometry& geometry);
  template<typename ValueType>
  void _readKeypoint( mtt::ValueKeypoint<ValueType,
                      mtt::AnimationTrack::TimeType>& keypoint);
  void _readCubemapData(mtt::CubemapObject& object);
  mtt::UID _readUID();

  static void _loadObjectData(mtt::Object& object,
                              mtt::DataStream& stream,
                              const QDir& fileDirectory,
                              mtt::UID::ValueType mixUIDValue);

private:
  mtt::DataStream& _stream;
  QDir _fileDirectory;
  mtt::UID::ValueType _mixUIDValue;
};

template<typename ObjectType>
inline std::unique_ptr<ObjectType> ObjectLoader::loadObject(
                                                bool canBeRenamed,
                                                mtt::DataStream& stream,
                                                QDir& fileDirectory,
                                                mtt::UID::ValueType mixUIDValue)
{
  uint16_t objectTypeIndex;
  stream >> objectTypeIndex;
  ObjectBuilder::ObjectType objectType =
                        static_cast<ObjectBuilder::ObjectType>(objectTypeIndex);

  mtt::UID id = stream.readUID().mixedUID(mixUIDValue);

  QString name;
  stream >> name;

  std::unique_ptr<ObjectType> object =
                ObjectBuilder::buildObject<ObjectType>( objectType,
                                                        name,
                                                        canBeRenamed,
                                                        id);

  _loadObjectData(*object, stream, fileDirectory, mixUIDValue);

  return object;
}
