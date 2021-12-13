#pragma once

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/Application/DataStream.h>

#include <Objects/OEVisitor.h>

class ObjectDataLoader : private OEVisitor
{
public:
  /// This method assumes the id and name have already been loaded from
  /// the stream
  static void loadObject( mtt::Object& object,
                          mtt::DataStream& stream,
                          const QDir& fileDirectory);

  template<typename ObjectType>
  inline static std::unique_ptr<ObjectType> loadObject( bool canBeRenamed,
                                                        mtt::DataStream& stream,
                                                        QDir& fileDirectory);

private:
  ObjectDataLoader(mtt::DataStream& stream, const QDir& fileDirectory);
  ObjectDataLoader(const ObjectDataLoader&) = delete;
  ObjectDataLoader& operator = (const ObjectDataLoader&) = delete;
  virtual ~ObjectDataLoader() noexcept = default;

  virtual void visit(AnimationObject& object) override;
  virtual void visit(AnimationTrack& object) override;
  virtual void visit(DisplayedObject& object) override;
  virtual void visit(GeometryObject& object) override;
  virtual void visit(LODObject& object) override;
  virtual void visit(MaterialObject& object) override;
  virtual void visit(MeshObject& object) override;
  virtual void visit(MovableObject& object) override;
  virtual void visit(RotatableObject& object) override;
  virtual void visit(ScalableObject& object) override;
  virtual void visit(SkeletonObject& object) override;

  QString loadFilename();
  std::unique_ptr<BoneRefBatch> readBoneRefs();
  void readGeometry(mtt::CommonMeshGeometry& geometry);
  template<typename ValueType>
  void readKeypoint(
            mtt::ValueKeypoint<ValueType, AnimationTrack::TimeType>& keypoint);

private:
  mtt::DataStream& _stream;
  QDir _fileDirectory;
};

template<typename ObjectType>
inline std::unique_ptr<ObjectType> ObjectDataLoader::loadObject(
                                                        bool canBeRenamed,
                                                        mtt::DataStream& stream,
                                                        QDir& fileDirectory)
{
  uint16_t objectType;
  stream >> objectType;

  mtt::UID id = stream.readUID();

  QString name;
  stream >> name;

  std::unique_ptr<ObjectType> object(new ObjectType(name, canBeRenamed, id));

  loadObject(*object, stream, fileDirectory);

  return object;
}
