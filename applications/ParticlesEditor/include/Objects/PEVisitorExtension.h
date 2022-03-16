#pragma once

#include <mtt/application/Scene/ObjectVisitor.h>

class EmitterObject;
class FrameObject;
class ModificatorGroup;
class ModificatorObject;
class ParticleAnimation;
class ParticleField;
class RootObject;

class PEVisitorExtension
{
public:
  static mtt::ObjectVisitor::ExtensionID extensionID;

public:
  virtual void visitConstEmitterObject(const EmitterObject& object) = 0;
  virtual void visitEmitterObject(EmitterObject& object) = 0;

  virtual void visitConstFrameObject(const FrameObject& object) = 0;
  virtual void visitFrameObject(FrameObject& object) = 0;

  virtual void visitConstModificatorGroup(const ModificatorGroup& object) = 0;
  virtual void visitModificatorGroup(ModificatorGroup& object) = 0;

  virtual void visitConstModificatorObject(const ModificatorObject& object) = 0;
  virtual void visitModificatorObject(ModificatorObject& object) = 0;

  virtual void visitConstParticleAnimation(const ParticleAnimation& object) = 0;
  virtual void visitParticleAnimation(ParticleAnimation& object) = 0;

  virtual void visitConstParticleField(const ParticleField& object) = 0;
  virtual void visitParticleField(ParticleField& object) = 0;

  virtual void visitConstRootObject(const RootObject& object) = 0;
  virtual void visitRootObject(RootObject& object) = 0;
};