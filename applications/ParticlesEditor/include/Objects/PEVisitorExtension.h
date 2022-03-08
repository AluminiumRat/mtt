#pragma once

#include <mtt/application/Scene/ObjectVisitor.h>

class FrameObject;
class ModificatorGroup;
class ModificatorObject;
class ParticleField;
class RootObject;

class PEVisitorExtension
{
public:
  static mtt::ObjectVisitor::ExtensionID extensionID;

public:
  virtual void visitConstFrameObject(const FrameObject& object) = 0;
  virtual void visitFrameObject(FrameObject& object) = 0;

  virtual void visitConstModificatorGroup(const ModificatorGroup& object) = 0;
  virtual void visitModificatorGroup(ModificatorGroup& object) = 0;

  virtual void visitConstModificatorObject(const ModificatorObject& object) = 0;
  virtual void visitModificatorObject(ModificatorObject& object) = 0;

  virtual void visitConstParticleField(const ParticleField& object) = 0;
  virtual void visitParticleField(ParticleField& object) = 0;

  virtual void visitConstRootObject(const RootObject& object) = 0;
  virtual void visitRootObject(RootObject& object) = 0;
};