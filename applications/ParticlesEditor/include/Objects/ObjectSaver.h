#pragma once

#include <mtt/editorLib/Objects/ObjectSaver.h>

#include <Objects/PEVisitor.h>

class ObjectSaver : public PEVisitorT<mtt::ObjectSaver>
{
public:
  ObjectSaver() = default;
  ObjectSaver(const ObjectSaver&) = delete;
  ObjectSaver& operator = (const ObjectSaver&) = delete;
  virtual ~ObjectSaver() noexcept = default;

  virtual void visitConstEmitterObject(const EmitterObject& object) override;
  virtual void visitConstFrameObject(const FrameObject& object) override;
  virtual void visitConstModificatorObject(
                                      const ModificatorObject& object) override;
  virtual void visitConstParticleAnimation(
                                      const ParticleAnimation& object) override;
  virtual void visitConstParticleField(const ParticleField& object) override;
  virtual void visitConstVisibilityControlObject(
                                const VisibilityControlObject& object) override;
};