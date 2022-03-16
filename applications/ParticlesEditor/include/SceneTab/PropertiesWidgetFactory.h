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

  virtual void visitEmitterObject(EmitterObject& object) override;
  virtual void visitModificatorObject(ModificatorObject& object) override;
  virtual void visitParticleAnimation(ParticleAnimation& object) override;
  virtual void visitParticleField(ParticleField& object) override;

private:
  EditorCommonData& _commonData;
};
