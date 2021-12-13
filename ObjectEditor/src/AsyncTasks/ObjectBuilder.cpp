#include <mtt/Utilities/Abort.h>

#include <AsyncTasks/ObjectBuilder.h>
#include <Objects/OEVisitor.h>

struct ObjectBuilder::ObjectTypeParser : public OEVisitor
{
  ObjectBuilder::ObjectType type = ObjectBuilder::ObjectType::Object;

  virtual void visit(const AmbientLightObject& object) override
  {
    type = ObjectBuilder::ObjectType::AmbientLight;
  }

  virtual void visit(const AnimationObject& object) override
  {
    type = ObjectBuilder::ObjectType::Animation;
  }

  virtual void visit(const AnimationTrack& object) override
  {
    type = ObjectBuilder::ObjectType::AnimationTrack;
  }

  virtual void visit(const BackgroundObject& object) override
  {
    type = ObjectBuilder::ObjectType::Background;
  }

  virtual void visit(const DirectLightObject& object) override
  {
    type = ObjectBuilder::ObjectType::DirectLight;
  }

  virtual void visit(const LODObject& object) override
  {
    type = ObjectBuilder::ObjectType::LOD;
  }

  virtual void visit(const MaterialObject& object) override
  {
    type = ObjectBuilder::ObjectType::Material;
  }

  virtual void visit(const MeshObject& object) override
  {
    type = ObjectBuilder::ObjectType::Mesh;
  }

  virtual void visit(const mtt::Object& object) override
  {
    type = ObjectBuilder::ObjectType::Object;
  }

  virtual void visit(const SkeletonObject& object) override
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
      return std::make_unique<AmbientLightObject>(name, canBeRenamed, id);

    case ObjectType::Background:
      return std::make_unique<BackgroundObject>(name, canBeRenamed, id);

    case ObjectType::DirectLight:
      return std::make_unique<DirectLightObject>(name, canBeRenamed, id);

    case ObjectType::Animation:
      return std::make_unique<AnimationObject>(name, canBeRenamed, id);

    case ObjectType::AnimationTrack:
      return std::make_unique<AnimationTrack>(name, canBeRenamed, id);

    case ObjectType::LOD:
      return std::make_unique<LODObject>(name, canBeRenamed, id);

    case ObjectType::Material:
      return std::make_unique<MaterialObject>(name, canBeRenamed, id);

    case ObjectType::Mesh:
      return std::make_unique<MeshObject>(name, canBeRenamed, id);

    case ObjectType::Object:
      return std::make_unique<mtt::Object>(name, canBeRenamed, id);

    case ObjectType::Skeleton:
      return std::make_unique<SkeletonObject>(name, canBeRenamed, id);
  }

  mtt::Abort("ObjectBuilder::_buildObject: unsupported object type");
}
