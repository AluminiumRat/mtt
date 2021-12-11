#include <stdexcept>

#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <mtt/Utilities/Log.h>

#include <AsyncTasks/SaveModelTask.h>
#include <Objects/OEVisitor.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

struct SaveModelTask::ObjectIdSaver : public OEVisitor
{
  mtt::DataStream* stream;

  virtual void visit(const AnimationObject& object) override
  {
    *stream << SaveModelTask::ObjectType::Animation;
  }

  virtual void visit(const AnimationTrack& object) override
  {
    *stream << SaveModelTask::ObjectType::AnimationTrack;
  }

  virtual void visit(const LODObject& object) override
  {
    *stream << SaveModelTask::ObjectType::LOD;
  }

  virtual void visit(const MaterialObject& object) override
  {
    *stream << SaveModelTask::ObjectType::Material;
  }

  virtual void visit(const MeshObject& object) override
  {
    *stream << SaveModelTask::ObjectType::Mesh;
  }

  virtual void visit(const mtt::Object& object) override
  {
    *stream << SaveModelTask::ObjectType::Object;
  }

  virtual void visit(const SkeletonObject& object) override
  {
    *stream << SaveModelTask::ObjectType::Skeleton;
  }
};

struct SaveModelTask::ObjectDataSaver : public OEVisitor
{
  mtt::DataStream* stream;
  QDir fileDirectory;

  virtual void visit(const AnimationObject& object) override
  {
    OEVisitor::visit(object);
    uint32_t childNumber = object.childsNumber();
    *stream << childNumber;
    for (uint32_t childIndex = 0; childIndex < childNumber; childIndex++)
    {
      SaveModelTask::_saveObject( object.child(childIndex),
                                  *stream,
                                  fileDirectory);
    }
  }

  template<typename ValueType>
  void writeKeypoint(
    mtt::ValueKeypoint<ValueType, AnimationTrack::TimeType> keypoint)
  {
    *stream << (uint32_t)keypoint.time().count();
    *stream << keypoint.value();
  }

  virtual void visit(const AnimationTrack& object) override
  {
    OEVisitor::visit(object);
    *stream << object.enabled();

    *stream <<(uint16_t)object.positionKeypointNumber();
    for ( size_t keypointIndex = 0;
          keypointIndex < object.positionKeypointNumber();
          keypointIndex++)
    {
      writeKeypoint(object.positionKeypoint(keypointIndex));
    }

    *stream <<(uint16_t)object.rotationKeypointNumber();
    for ( size_t keypointIndex = 0;
          keypointIndex < object.rotationKeypointNumber();
          keypointIndex++)
    {
      writeKeypoint(object.rotationKeypoint(keypointIndex));
    }

    *stream <<(uint16_t)object.scaleKeypointNumber();
    for ( size_t keypointIndex = 0;
          keypointIndex < object.scaleKeypointNumber();
          keypointIndex++)
    {
      writeKeypoint(object.scaleKeypoint(keypointIndex));
    }
  
    *stream << object.skeletonRef().referencedId();
  }

  virtual void visit(const DisplayedObject& object) override
  {
    OEVisitor::visit(object);
    *stream << object.visible();
  }

  virtual void visit(const GeometryObject& object) override
  {
    OEVisitor::visit(object);
    *stream << object.skeletonRef().referencedId();
  }

  virtual void visit(const LODObject& object) override
  {
    OEVisitor::visit(object);
    *stream << object.minMppx();
    *stream << object.maxMppx();

    uint32_t childNumber = object.childsNumber();
    *stream << childNumber;
    for (uint32_t childIndex = 0; childIndex < childNumber; childIndex++)
    {
      SaveModelTask::_saveObject( object.child(childIndex),
                                  *stream,
                                  fileDirectory);
    }
  }

  void writeFilename(const QString& filename)
  {
    *stream << fileDirectory.relativeFilePath(filename);
  }

  virtual void visit(const MaterialObject& object) override
  {
    OEVisitor::visit(object);
    
    *stream << object.albedo();
    writeFilename(object.albedoTexture());
    *stream << object.useAlphaFromAlbedoTexture();
    
    *stream << object.roughness();
    *stream << object.specularStrength();
    writeFilename(object.specularTexture());

    *stream << object.metallic();
    
    *stream << object.opaqueFactor();
    writeFilename(object.opaqueTexture());

    *stream << object.reflectionFactor();
    writeFilename(object.reflectionTexture());

    *stream << object.emissionColor();
    *stream << object.emissionFactor();
    writeFilename(object.emissionTexture());

    writeFilename(object.normalTexture());
  }

  void saveGeometry(const mtt::CommonMeshGeometry& geometry)
  {
    *stream << geometry.positions;
    *stream << geometry.normals;
    *stream << geometry.tangents;
    *stream << geometry.binormals;
    *stream << geometry.texCoords;
    uint64_t skeletonRefsNumber = geometry.skeletonRefs.size();
    *stream << skeletonRefsNumber;
    for (const mtt::CommonMeshGeometry::SkeletonRef& skeletonRef :
                                                          geometry.skeletonRefs)
    {
      *stream << skeletonRef.bonesNumber;
      for ( uint16_t boneIndex = 0;
            boneIndex < skeletonRef.bonesNumber;
            boneIndex++)
      {
        *stream << skeletonRef.bones[boneIndex].boneIndex;
        *stream << skeletonRef.bones[boneIndex].weight;
      }
    }

    *stream << geometry.triangleIndices;
    *stream << geometry.lineIndices;
  }

  void saveBoneRefs(const BoneRefBatch& refs)
  {
    *stream << (uint16_t) refs.boneRefsNumber();
    for (size_t refIndex = 0; refIndex < refs.boneRefsNumber(); refIndex++)
    {
      const BoneRefObject& boneRef = refs.boneRef(refIndex);
      *stream << boneRef.name();
      *stream << boneRef.boneRef().referencedId();
      *stream << boneRef.boneInverseMatrix();
    }
  }

  virtual void visit(const MeshObject& object) override
  {
    OEVisitor::visit(object);

    const mtt::CommonMeshGeometry& geometry = object.geometry();
    saveGeometry(geometry);
    saveBoneRefs(object.boneRefs());
    *stream << object.materialRef().referencedId();
  }

  virtual void visit(const MovableObject& object) override
  {
    OEVisitor::visit(object);
    *stream << object.position();
  }

  virtual void visit(const mtt::Object& object) override
  {
    *stream << object.id();
    *stream << object.name();
  }

  virtual void visit(const RotatableObject& object) override
  {
    OEVisitor::visit(object);
    *stream << object.rotation();
  }

  virtual void visit(const ScalableObject& object) override
  {
    OEVisitor::visit(object);
    *stream << object.scale();
  }

  virtual void visit(const SkeletonObject& object) override
  {
    OEVisitor::visit(object);
    uint32_t childNumber = object.childsNumber();
    *stream << childNumber;
    for (uint32_t childIndex = 0; childIndex < childNumber; childIndex++)
    {
      SaveModelTask::_saveObject( object.child(childIndex),
                                  *stream,
                                  fileDirectory);
    }
  }
};

SaveModelTask::SaveModelTask( const EditorScene& scene,
                              const QString& filename,
                              EditorCommonData& commonData) :
  AbstractAsyncTask("Save",
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

void SaveModelTask::asyncPart()
{
  if(!_fileDirectory.exists()) throw std::runtime_error("The file directory does not exist");

  QFile file(_filename);
  if(!file.open(QFile::WriteOnly)) throw std::runtime_error("Unable to open model file");
  _file = &file;

  mtt::DataStream stream(&file);
  _stream = &stream;

  _writeHead();
  _writeMaterials();
  _writeSkeletons();
  _writeGeometry();
  _writeAnimations();
}

void SaveModelTask::_writeHead()
{
  _file->write(fileHead.c_str(), fileHead.length());
  *_stream << fileVersion;
}

void SaveModelTask::_saveObject(const mtt::Object& object,
                                mtt::DataStream& stream,
                                const QDir& fileDirectory)
{
  ObjectIdSaver idSaver;
  idSaver.stream = &stream;
  idSaver.process(object);

  ObjectDataSaver dataSaver;
  dataSaver.stream = &stream;
  dataSaver.fileDirectory = fileDirectory;
  dataSaver.process(object);
}

void SaveModelTask::_writeMaterials()
{
  uint32_t materialsNumber = _scene.root().materialsGroup().childsNumber();
  *_stream << materialsNumber;
  for(uint32_t materialIndex = 0;
      materialIndex < materialsNumber;
      materialIndex++)
  {
    _saveObject(_scene.root().materialsGroup().child(materialIndex),
                *_stream,
                _fileDirectory);
  }
}

void SaveModelTask::_writeSkeletons()
{
  uint32_t skeletonsNumber = _scene.root().skeletonGroup().childsNumber();
  *_stream << skeletonsNumber;
  for(uint32_t skeletonIndex = 0;
      skeletonIndex < skeletonsNumber;
      skeletonIndex++)
  {
    _saveObject(_scene.root().skeletonGroup().child(skeletonIndex),
                *_stream,
                _fileDirectory);
  }
}

void SaveModelTask::_writeGeometry()
{
  uint32_t lodsNumber = _scene.root().geometryGroup().childsNumber();
  *_stream << lodsNumber;
  for(uint32_t lodIndex = 0;
      lodIndex < lodsNumber;
      lodIndex++)
  {
    _saveObject(_scene.root().geometryGroup().child(lodIndex),
                *_stream,
                _fileDirectory);
  }
}

void SaveModelTask::_writeAnimations()
{
  uint32_t animationsNumber = _scene.root().animationGroup().childsNumber();
  *_stream << animationsNumber;
  for(uint32_t animationIndex = 0;
      animationIndex < animationsNumber;
      animationIndex++)
  {
    _saveObject(_scene.root().animationGroup().child(animationIndex),
                *_stream,
                _fileDirectory);
  }
}

void SaveModelTask::finalizePart()
{
  _commonData.setModelFilename(_filename);
}
