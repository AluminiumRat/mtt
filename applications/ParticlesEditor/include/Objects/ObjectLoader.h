#pragma once

#include <mtt/editorLib/Objects/ObjectLoader.h>

#include <Objects/PEVisitor.h>

class ObjectLoader : public PEVisitorT<mtt::ObjectLoader>
{
public:
  ObjectLoader() = default;
  ObjectLoader(const ObjectLoader&) = delete;
  ObjectLoader& operator = (const ObjectLoader&) = delete;
  virtual ~ObjectLoader() noexcept = default;

  virtual void visitEmitterObject(EmitterObject& object) override;
  virtual void visitFrameObject(FrameObject& object) override;
  virtual void visitModificatorObject(ModificatorObject& object) override;
  virtual void visitParticleAnimation(ParticleAnimation& object) override;
  virtual void visitParticleField(ParticleField& object) override;
};