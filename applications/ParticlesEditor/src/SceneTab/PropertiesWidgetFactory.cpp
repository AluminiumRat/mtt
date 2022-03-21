#include <QtWidgets/QVBoxLayout>

#include <mtt/application/Widgets/PropertiesWidgets/BoolPropertyWidget.h>

#include <SceneTab/EmitterWidget.h>
#include <SceneTab/FluidWidget.h>
#include <SceneTab/GravityWidget.h>
#include <SceneTab/ParticleAnimationWidget.h>
#include <SceneTab/ParticleFieldWidget.h>
#include <SceneTab/PropertiesWidgetFactory.h>
#include <SceneTab/TypeMaskWidget.h>
#include <SceneTab/VisibilityControlWidget.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

PropertiesWidgetFactory::PropertiesWidgetFactory(
                                                QVBoxLayout& widgetsLayout,
                                                EditorCommonData& commonData) :
  PEVisitorT( widgetsLayout,
              commonData,
              &commonData.scene()->root().modificatorsGroup(),
              &commonData.animationPlayer()),
  _commonData(commonData)
{
}

void PropertiesWidgetFactory::visitEmitterObject(EmitterObject& object)
{
  PEVisitorT::visitEmitterObject(object);
  widgetsLayout().addWidget(new EmitterWidget(object, _commonData.undoStack()));
  widgetsLayout().addWidget(
                          new TypeMaskWidget(object, _commonData.undoStack()));
}

void PropertiesWidgetFactory::visitFluidObject(FluidObject& object)
{
  PEVisitorT::visitFluidObject(object);
  widgetsLayout().addWidget(new FluidWidget(object, _commonData.undoStack()));
  widgetsLayout().addWidget(
                          new TypeMaskWidget(object, _commonData.undoStack()));
}

void PropertiesWidgetFactory::visitGravityModificator(
                                                    GravityModificator& object)
{
  PEVisitorT::visitGravityModificator(object);
  widgetsLayout().addWidget(new GravityWidget(object, _commonData.undoStack()));
  widgetsLayout().addWidget(
                          new TypeMaskWidget(object, _commonData.undoStack()));
}

void PropertiesWidgetFactory::visitModificatorObject(ModificatorObject& object)
{
  PEVisitorT::visitModificatorObject(object);
  using EnabledWidget = mtt::BoolPropertyWidget<ModificatorObject>;
  std::unique_ptr<EnabledWidget> enableWidget(
                          new EnabledWidget(object,
                                            &ModificatorObject::enabled,
                                            &ModificatorObject::setEnabled,
                                            &ModificatorObject::enabledChanged,
                                            _commonData.undoStack()));
  enableWidget->setText(QObject::tr("Enabled"));
  widgetsLayout().addWidget(enableWidget.release());
}

void PropertiesWidgetFactory::visitParticleAnimation(ParticleAnimation& object)
{
  PEVisitorT::visitParticleAnimation(object);
  widgetsLayout().addWidget(
                          new ParticleAnimationWidget(object,
                                                      _commonData.undoStack()));
}

void PropertiesWidgetFactory::visitParticleField(ParticleField& object)
{
  PEVisitorT::visitParticleField(object);
  widgetsLayout().addWidget(new ParticleFieldWidget(object,
                                                    _commonData.undoStack()));
}

void PropertiesWidgetFactory::visitVisibilityControlObject(
                                                VisibilityControlObject& object)
{
  PEVisitorT::visitVisibilityControlObject(object);
  widgetsLayout().addWidget(new VisibilityControlWidget(
                                                      object,
                                                      _commonData.undoStack()));
  widgetsLayout().addWidget(new TypeMaskWidget(object, _commonData.undoStack()));
}
