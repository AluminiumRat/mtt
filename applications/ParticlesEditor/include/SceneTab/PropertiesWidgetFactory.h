#pragma once

#include <mtt/editorLib/SceneTab/PropertiesWidgetFactory.h>

#include <Objects/PEVisitor.h>

class EditorCommonData;
class EditorScene;

class PropertiesWidgetFactory : public PEVisitorT<mtt::PropertiesWidgetFactory>
{
public:
  PropertiesWidgetFactory(QVBoxLayout& widgetsLayout,
                          EditorCommonData& commonData);
  PropertiesWidgetFactory(const PropertiesWidgetFactory&) = delete;
  PropertiesWidgetFactory& operator = (
                                      const PropertiesWidgetFactory&) = delete;
  virtual ~PropertiesWidgetFactory() noexcept = default;

  virtual void visitBlockerObject(BlockerObject& object) override;
  virtual void visitEmitterObject(EmitterObject& object) override;
  virtual void visitFluidObject(FluidObject& object) override;
  virtual void visitGasSource(GasSource& object) override;
  virtual void visitGravityModificator(GravityModificator& object) override;
  virtual void visitHeaterObject(HeaterObject& object) override;
  virtual void visitModificatorObject(ModificatorObject& object) override;
  virtual void visitParticleAnimation(ParticleAnimation& object) override;
  virtual void visitParticleField(ParticleField& object) override;
  virtual void visitVisibilityControlObject(
                                      VisibilityControlObject& object) override;

private:
  EditorCommonData& _commonData;
};
