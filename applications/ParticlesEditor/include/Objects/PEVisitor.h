#pragma once

#include <mtt/editorLib/Objects/CEVisitor.h>

#include <Objects/ParticleField.h>
#include <Objects/PEVisitorExtension.h>
#include <Objects/RootObject.h>

template <typename BaseVisitor>
class PEVisitorT :  public BaseVisitor,
                    public PEVisitorExtension
{
public:
  template <typename... Args>
  inline PEVisitorT(Args&&... args);
  PEVisitorT(const PEVisitorT&) = delete;
  PEVisitorT& operator = (const PEVisitorT&) = delete;
  virtual ~PEVisitorT() noexcept = default;

  virtual void visitConstParticleField(const ParticleField& object) override;
  virtual void visitParticleField(ParticleField& object) override;

  virtual void visitConstRootObject(const RootObject& object) override;
  virtual void visitRootObject(RootObject& object) override;

protected:
  inline virtual void* getExtension(
                          mtt::ObjectVisitor::ExtensionID id) noexcept override;
};

using PEVisitor = PEVisitorT<mtt::CEVisitor>;

template <typename BaseVisitor>
template <typename... Args>
inline PEVisitorT<BaseVisitor>::PEVisitorT(Args&&... args) :
  BaseVisitor(args...)
{
}

template <typename BaseVisitor>
void PEVisitorT<BaseVisitor>::visitConstParticleField(
                                                    const ParticleField& object)
{
  static_cast<BaseVisitor*>(this)->visitConstMovableObject(object);
}

template <typename BaseVisitor>
void PEVisitorT<BaseVisitor>::visitParticleField(ParticleField& object)
{
  static_cast<BaseVisitor*>(this)->visitMovableObject(object);
}

template <typename BaseVisitor>
void PEVisitorT<BaseVisitor>::visitConstRootObject(const RootObject& object)
{
  static_cast<BaseVisitor*>(this)->visitConstObject(object);
}

template <typename BaseVisitor>
void PEVisitorT<BaseVisitor>::visitRootObject(RootObject& object)
{
  static_cast<BaseVisitor*>(this)->visitObject(object);
}

template <typename BaseVisitor>
inline void* PEVisitorT<BaseVisitor>::getExtension(
                                  mtt::ObjectVisitor::ExtensionID id) noexcept
{
  if(id == PEVisitorExtension::extensionID)
  {
    PEVisitorExtension* extension = this;
    return extension;
  }
  else return BaseVisitor::getExtension(id);
}
