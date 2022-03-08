#pragma once

#include <mtt/editorLib/Objects/CEVisitor.h>

#include <Objects/FrameObject.h>
#include <Objects/EmitterObject.h>
#include <Objects/ModificatorGroup.h>
#include <Objects/ModificatorObject.h>
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

  inline virtual void visitConstEmitterObject(
                                          const EmitterObject& object) override;
  inline virtual void visitEmitterObject(EmitterObject& object) override;

  inline virtual void visitConstFrameObject(const FrameObject& object) override;
  inline virtual void visitFrameObject(FrameObject& object) override;

  inline virtual void visitConstModificatorGroup(
                                      const ModificatorGroup& object) override;
  inline virtual void visitModificatorGroup(ModificatorGroup& object) override;

  inline virtual void visitConstModificatorObject(
                                      const ModificatorObject& object) override;
  inline virtual void visitModificatorObject(
                                            ModificatorObject& object) override;

  inline virtual void visitConstParticleField(
                                          const ParticleField& object) override;
  inline virtual void visitParticleField(ParticleField& object) override;

  inline virtual void visitConstRootObject(const RootObject& object) override;
  inline virtual void visitRootObject(RootObject& object) override;

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
inline void PEVisitorT<BaseVisitor>::visitConstEmitterObject(
                                                    const EmitterObject& object)
{
  visitConstModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitEmitterObject(EmitterObject& object)
{
  visitModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstFrameObject(
                                                      const FrameObject& object)
{
  visitConstModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitFrameObject(FrameObject& object)
{
  visitModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstModificatorGroup(
                                                const ModificatorGroup& object)
{
  static_cast<BaseVisitor*>(this)->visitConstDisplayedObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitModificatorGroup(
                                                      ModificatorGroup& object)
{
  static_cast<BaseVisitor*>(this)->visitDisplayedObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstModificatorObject(
                                                const ModificatorObject& object)
{
  static_cast<BaseVisitor*>(this)->visitConstScalableObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitModificatorObject(
                                                      ModificatorObject& object)
{
  static_cast<BaseVisitor*>(this)->visitScalableObject(object);
}

template <typename BaseVisitor>
inline  void PEVisitorT<BaseVisitor>::visitConstParticleField(
                                                    const ParticleField& object)
{
  static_cast<BaseVisitor*>(this)->visitConstMovableObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitParticleField(ParticleField& object)
{
  static_cast<BaseVisitor*>(this)->visitMovableObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstRootObject(
                                                      const RootObject& object)
{
  static_cast<BaseVisitor*>(this)->visitConstObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitRootObject(RootObject& object)
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
