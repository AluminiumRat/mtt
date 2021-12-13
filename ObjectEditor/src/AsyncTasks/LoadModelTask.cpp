#include <stdexcept>

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <mtt/Utilities/Log.h>

#include <AsyncTasks/LoadModelTask.h>
#include <AsyncTasks/SaveModelTask.h>
#include <Objects/OEVisitor.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

struct LoadModelTask::ObjectLoader : public OEVisitor
{
  mtt::DataStream* stream;
  QDir fileDirectory;

  virtual void visit(AnimationObject& object) override
  {
    OEVisitor::visit(object);
    uint32_t childNumber = stream->readUint32();
    for (; childNumber != 0; childNumber--)
    {
      object.addChild(
            LoadModelTask::_loadObject<AnimationTrack>(*stream, fileDirectory));
    }
  }

  template<typename ValueType>
  void readKeypoint(
              mtt::ValueKeypoint<ValueType, AnimationTrack::TimeType>& keypoint)
  {
    uint32_t timeCount = stream->readUint32();
    AnimationTrack::TimeType time(timeCount);
    keypoint.setTime(time);

    ValueType value;
    *stream >> value;
    keypoint.setValue(value);

    uint8_t interpolation = stream->readInt8();
    keypoint.setInterpolation(mtt::InterpolationType(interpolation));
  }

  virtual void visit(AnimationTrack& object) override
  {
    OEVisitor::visit(object);
    object.setEnabled(stream->readBool());

    uint16_t keypointsNumber = stream->readUint16();
    for (; keypointsNumber != 0; keypointsNumber--)
    {
      std::unique_ptr<AnimationTrack::PositionKeypoint> keypoint(
                                        new AnimationTrack::PositionKeypoint());
      readKeypoint(*keypoint);
      object.addPositionKeypoint(std::move(keypoint));
    }

    keypointsNumber = stream->readUint16();
    for (; keypointsNumber != 0; keypointsNumber--)
    {
      std::unique_ptr<AnimationTrack::RotationKeypoint> keypoint(
                                        new AnimationTrack::RotationKeypoint());
      readKeypoint(*keypoint);
      object.addRotationKeypoint(std::move(keypoint));
    }

    keypointsNumber = stream->readUint16();
    for (; keypointsNumber != 0; keypointsNumber--)
    {
      std::unique_ptr<AnimationTrack::ScaleKeypoint> keypoint(
                                          new AnimationTrack::ScaleKeypoint());
      readKeypoint(*keypoint);
      object.addScaleKeypoint(std::move(keypoint));
    }

    object.setSkeletonId(stream->readUID());
  }

  virtual void visit(DisplayedObject& object) override
  {
    OEVisitor::visit(object);
    object.setVisible(stream->readBool());
  }

  virtual void visit(GeometryObject& object) override
  {
    OEVisitor::visit(object);
    object.setSkeletonId(stream->readUID());
  }

  virtual void visit(LODObject& object) override
  {
    OEVisitor::visit(object);
    object.setMinMppx(stream->readFloat());
    object.setMaxMppx(stream->readFloat());

    uint32_t childNumber = stream->readUint32();
    for (; childNumber != 0; childNumber--)
    {
      object.addChild(
                LoadModelTask::_loadObject<MeshObject>(*stream, fileDirectory));
    }
  }

  QString loadFilename()
  {
    QString relativePath;
    *stream >> relativePath;
    if(relativePath.isEmpty()) return QString();
    QFileInfo fileInfo(fileDirectory.absoluteFilePath(relativePath));
    return fileInfo.canonicalFilePath();
  }

  virtual void visit(MaterialObject& object) override
  {
    OEVisitor::visit(object);

    object.setAlbedo(stream->readVec3());
    object.setAlbedoTexture(loadFilename());
    object.setUseAlphaFromAlbedoTexture(stream->readBool());
  
    object.setRoughness(stream->readFloat());
    object.setSpecularStrength(stream->readFloat());
    object.setSpecularTexture(loadFilename());

    object.setMetallic(stream->readFloat());
  
    object.setOpaqueFactor(stream->readFloat());
    object.setOpaqueTexture(loadFilename());

    object.setReflectionFactor(stream->readFloat());
    object.setReflectionTexture(loadFilename());

    object.setEmissionColor(stream->readVec3());
    object.setEmissionFactor(stream->readFloat());
    object.setEmissionTexture(loadFilename());

    object.setNormalTexture(loadFilename());
  }

  void readGeometry(mtt::CommonMeshGeometry& geometry)
  {
    *stream >> geometry.positions;
    *stream >> geometry.normals;
    *stream >> geometry.tangents;
    *stream >> geometry.binormals;
    *stream >> geometry.texCoords;
    uint64_t skeletonRefsNumber = stream->readUint64();
    geometry.skeletonRefs.resize(skeletonRefsNumber);
    for (mtt::CommonMeshGeometry::SkeletonRef& skeletonRef :
                                                          geometry.skeletonRefs)
    {
      *stream >> skeletonRef.bonesNumber;
      for ( uint16_t boneIndex = 0;
            boneIndex < skeletonRef.bonesNumber;
            boneIndex++)
      {
        *stream >> skeletonRef.bones[boneIndex].boneIndex;
        *stream >> skeletonRef.bones[boneIndex].weight;
      }
    }

    *stream >> geometry.triangleIndices;
    *stream >> geometry.lineIndices;
  }

  std::unique_ptr<BoneRefBatch> readBoneRefs()
  {
    std::vector<std::unique_ptr<BoneRefObject>> refs;

    uint16_t boneRefsNumber = stream->readUint16();
    for (;boneRefsNumber != 0; boneRefsNumber--)
    {
      QString name;
      *stream >> name;

      std::unique_ptr<BoneRefObject> refObject(new BoneRefObject(name, true));
      refObject->setBoneId(stream->readUID());
      refObject->setBoneInverseMatrix(stream->readMat4());
      refs.push_back(std::move(refObject));
    }

    return std::make_unique<BoneRefBatch>(
                                        std::move(refs),
                                        QCoreApplication::tr("Bone references"),
                                        false);
  }

  virtual void visit(MeshObject& object) override
  {
    OEVisitor::visit(object);

    mtt::CommonMeshGeometry geometry;
    readGeometry(geometry);
    object.setGeometry(std::move(geometry));
    object.setBoneRefs(readBoneRefs());
    object.setMaterialId(stream->readUID());
  }

  virtual void visit(MovableObject& object) override
  {
    OEVisitor::visit(object);
    object.setPosition(stream->readVec3());
  }

  virtual void visit(RotatableObject& object) override
  {
    OEVisitor::visit(object);
    object.setRotation(stream->readQuat());
  }

  virtual void visit(ScalableObject& object) override
  {
    OEVisitor::visit(object);
    object.setScale(stream->readVec3());
  }

  virtual void visit(SkeletonObject& object) override
  {
    OEVisitor::visit(object);
    uint32_t childsNumber = stream->readUint32();
    for (; childsNumber != 0; childsNumber--)
    {
      object.addChild(LoadModelTask::_loadObject<SkeletonObject>(
                                                                *stream,
                                                                fileDirectory));
    }
  }
};

LoadModelTask::LoadModelTask( EditorScene& scene,
                              const QString& filename,
                              EditorCommonData& commonData) :
  AbstractAsyncTask("Load",
                    mtt::AbstractAsyncTask::DEPENDENT,
                    mtt::AbstractAsyncTask::EXPLICIT),
  _scene(scene),
  _filename(filename),
  _file(nullptr),
  _fileDirectory(QFileInfo(_filename).dir()),
  _stream(nullptr),
  _commonData(commonData)
{
}

void LoadModelTask::_checkHead()
{
  std::string head;
  head.resize(SaveModelTask::fileHead.size());
  _file->read(head.data(), head.size());
  if(head != SaveModelTask::fileHead) throw std::runtime_error("Invalid mmd file header");
  uint32_t fileVersion;
  *_stream >> fileVersion;
  if(fileVersion > SaveModelTask::fileVersion) throw std::runtime_error("Unsupported version of mmd file");
}

template<typename ObjectType>
std::unique_ptr<ObjectType> LoadModelTask::_loadObject( mtt::DataStream& stream,
                                                        QDir& fileDirectory)
{
  uint16_t objectType;
  stream >> objectType;

  mtt::UID id = stream.readUID();

  QString name;
  stream >> name;

  std::unique_ptr<ObjectType> object(new ObjectType(name, true, id));

  ObjectLoader loader;
  loader.stream = &stream;
  loader.fileDirectory = fileDirectory;
  loader.process(*object);

  return object;
}

void LoadModelTask::_loadMaterials()
{
  uint32_t materialsNumber = _stream->readUint32();
  for (; materialsNumber != 0; materialsNumber--)
  {
    _materials.push_back(_loadObject<MaterialObject>(*_stream, _fileDirectory));
  }
}

void LoadModelTask::_loadSkeletons()
{
  uint32_t skeletonsNumber = _stream->readUint32();
  for (; skeletonsNumber != 0; skeletonsNumber--)
  {
    _skeletons.push_back(_loadObject<SkeletonObject>(*_stream, _fileDirectory));
  }
}

void LoadModelTask::_loadGeometry()
{
  uint32_t lodsNumber = _stream->readUint32();
  for (; lodsNumber != 0; lodsNumber--)
  {
    _lods.push_back(_loadObject<LODObject>(*_stream, _fileDirectory));
  }
}

void LoadModelTask::_loadAnimations()
{
  uint32_t animationsNumber = _stream->readUint32();
  for (; animationsNumber != 0; animationsNumber--)
  {
    _animations.push_back(
                        _loadObject<AnimationObject>(*_stream, _fileDirectory));
  }
}

void LoadModelTask::asyncPart()
{
  if(!_fileDirectory.exists()) throw std::runtime_error("The file directory does not exist");

  QFile file(_filename);
  if(!file.open(QFile::ReadOnly)) throw std::runtime_error("Unable to open model file");
  _file = &file;

  mtt::DataStream stream(&file);
  _stream = &stream;

  _checkHead();
  _loadMaterials();
  _loadSkeletons();
  _loadGeometry();
  _loadAnimations();
}

void LoadModelTask::_clearScene() noexcept
{
  AnimationGroup& animations = _scene.root().animationGroup();
  while (animations.childsNumber() != 0)
  {
    animations.removeChild( animations.child(animations.childsNumber() -1),
                            true);
  }

  GeometryGroup& geometry = _scene.root().geometryGroup();
  while (geometry.childsNumber() != 0)
  {
    geometry.removeChild( geometry.child(geometry.childsNumber() -1),
                          true);
  }

  MaterialsGroup& materials = _scene.root().materialsGroup();
  while (materials.childsNumber() != 0)
  {
    materials.removeChild(materials.child(materials.childsNumber() -1),
                          true);
  }

  SkeletonGroup& skeletons = _scene.root().skeletonGroup();
  while (skeletons.childsNumber() != 0)
  {
    skeletons.removeChild(skeletons.child(skeletons.childsNumber() -1),
                          true);
  }
}

void LoadModelTask::finalizePart()
{
  _commonData.undoStack().clear();
  _commonData.setModelFilename("");
  _clearScene();

  try
  {
    for (std::unique_ptr<AnimationObject>& animation : _animations)
    {
      _scene.root().animationGroup().addChild(std::move(animation));
    }

    for (std::unique_ptr<LODObject>& lod : _lods)
    {
      _scene.root().geometryGroup().addChild(std::move(lod));
    }

    for (std::unique_ptr<MaterialObject>& material : _materials)
    {
      _scene.root().materialsGroup().addChild(std::move(material));
    }

    for (std::unique_ptr<SkeletonObject>& skeleton : _skeletons)
    {
      _scene.root().skeletonGroup().addChild(std::move(skeleton));
    }

    _commonData.setModelFilename(_filename);
  }
  catch (...)
  {
    _clearScene();
    throw;
  }
}