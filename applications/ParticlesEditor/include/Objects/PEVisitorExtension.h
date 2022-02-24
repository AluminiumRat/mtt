#pragma once

#include <mtt/application/Scene/ObjectVisitor.h>

class ParticleField;
class RootObject;

class PEVisitorExtension
{
public:
  static mtt::ObjectVisitor::ExtensionID extensionID;

public:
  virtual void visitConstParticleField(const ParticleField& object) = 0;
  virtual void visitParticleField(ParticleField& object) = 0;

  virtual void visitConstRootObject(const RootObject& object) = 0;
  virtual void visitRootObject(RootObject& object) = 0;
};