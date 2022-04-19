#pragma once

#include <mtt/editorLib/PropertiesWidget/PropertiesWidgetFactory.h>

#include <Objects/PEVisitor.h>

class ParticlesEditorCommonData;

class PropertiesWidgetFactory : public PEVisitorT<mtt::PropertiesWidgetFactory>
{
public:
  PropertiesWidgetFactory(QVBoxLayout& widgetsLayout,
                          ParticlesEditorCommonData& commonData);
  PropertiesWidgetFactory(const PropertiesWidgetFactory&) = delete;
  PropertiesWidgetFactory& operator = (
                                      const PropertiesWidgetFactory&) = delete;
  virtual ~PropertiesWidgetFactory() noexcept = default;

  virtual void visitAnimationAction(AnimationAction& object) override;
  virtual void visitBlockerObject(BlockerObject& object) override;
  virtual void visitEmitterObject(EmitterObject& object) override;
  virtual void visitFluidObject(FluidObject& object) override;
  virtual void visitGasSource(GasSource& object) override;
  virtual void visitGravityModificator(GravityModificator& object) override;
  virtual void visitHeaterObject(HeaterObject& object) override;
  virtual void visitModificatorObject(ModificatorObject& object) override;
  virtual void visitParticleAnimation(ParticleAnimation& object) override;
  virtual void visitParticleField(ParticleField& object) override;
  virtual void visitParticlesEmissionAction(
                                      ParticlesEmissionAction& object) override;
  virtual void visitSizeControlObject(SizeControlObject& object) override;
  virtual void visitVisibilityControlObject(
                                      VisibilityControlObject& object) override;

private:
  ParticlesEditorCommonData& _commonData;
};
