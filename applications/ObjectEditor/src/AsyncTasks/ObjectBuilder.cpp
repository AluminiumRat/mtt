#include <mtt/utilities/Abort.h>

#include <AsyncTasks/ObjectBuilder.h>
#include <Objects/OEVisitor.h>

struct ObjectBuilder::ObjectTypeParser : public OEVisitor
{
  ObjectBuilder::ObjectType type = ObjectBuilder::ObjectType::Object;

  virtual void visitConstAmbientLightObject(
                                const mtt::AmbientLightObject& object) override
  {
    type = ObjectBuilder::ObjectType::AmbientLight;
  }

  virtual void visitConstAnimationObject(
                                    const mtt::AnimationObject& object) override
  {
    type = ObjectBuilder::ObjectType::Animation;
  }

  virtual void visitConstAnimationTrack(
                                    const mtt::AnimationTrack& object) override
  {
    type = ObjectBuilder::ObjectType::AnimationTrack;
  }

  virtual void visitConstBackgroundObject(
                                  const mtt::BackgroundObject& object) override
  {
    type = ObjectBuilder::ObjectType::Background;
  }

  virtual void visitConstDirectLightObject(
                                  const mtt::DirectLightObject& object) override
  {
    type = ObjectBuilder::ObjectType::DirectLight;
  }

  virtual void visitConstEnvironmentObject(
                                  const mtt::EnvironmentObject& object) override
  {
    type = ObjectBuilder::ObjectType::EnvironmentModel;
  }

  virtual void visitConstLODObject(const LODObject& object) override
  {
    type = ObjectBuilder::ObjectType::LOD;
  }

  virtual void visitConstMaterialObject(const MaterialObject& object) override
  {
    type = ObjectBuilder::ObjectType::Material;
  }

  virtual void visitConstMeshObject(const MeshObject& object) override
  {
    type = ObjectBuilder::ObjectType::Mesh;
  }

  virtual void visitConstObject(const mtt::Object& object) override
  {
    type = ObjectBuilder::ObjectType::Object;
  }

  virtual void visitConstSkeletonObject(
                                    const mtt::SkeletonObject& object) override
  {
    type = ObjectBuilder::ObjectType::Skeleton;
  }
};

ObjectBuilder::ObjectType ObjectBuilder::getObjectType(
                                            const mtt::Object& object) noexcept
{
  ObjectTypeParser parser;
  parser.process(object);
  if(parser.type == ObjectType::Object) mtt::Abort("ObjectBuilder::getObjectType: unsupported object class.");
  return parser.type;
}

std::unique_ptr<mtt::Object> ObjectBuilder::_buildObject( ObjectType type,
                                                          QString name,
                                                          bool canBeRenamed,
                                                          mtt::UID id)
{
  switch (type)
  {
    case ObjectType::AmbientLight:
      return std::make_unique<mtt::AmbientLightObject>(name, canBeRenamed, id);

    case ObjectType::Animation:
      return std::make_unique<mtt::AnimationObject>(name, canBeRenamed, id);

    case ObjectType::AnimationTrack:
      return std::make_unique<mtt::AnimationTrack>(name, canBeRenamed, id);

    case ObjectType::Background:
      return std::make_unique<mtt::BackgroundObject>(name, canBeRenamed, id);

    case ObjectType::DirectLight:
      return std::make_unique<mtt::DirectLightObject>(name, canBeRenamed, id);

    case ObjectType::EnvironmentModel:
      return std::make_unique<mtt::EnvironmentModel>(name, canBeRenamed, id);

    case ObjectType::LOD:
      return std::make_unique<LODObject>(name, canBeRenamed, id);

    case ObjectType::Material:
      return std::make_unique<MaterialObject>(name, canBeRenamed, id);

    case ObjectType::Mesh:
      return std::make_unique<MeshObject>(name, canBeRenamed, id);

    case ObjectType::Object:
      return std::make_unique<mtt::Object>(name, canBeRenamed, id);

    case ObjectType::Skeleton:
      return std::make_unique<mtt::SkeletonObject>(name, canBeRenamed, id);
  }

  mtt::Abort("ObjectBuilder::_buildObject: unsupported object type");
}
