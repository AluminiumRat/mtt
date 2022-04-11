#include <QtWidgets/QVBoxLayout>

#include <mtt/application/Widgets/PropertiesWidgets/BoolPropertyWidget.h>

#include <SceneTab/BlockerWidget.h>
#include <SceneTab/EmitterWidget.h>
#include <SceneTab/FluidWidget.h>
#include <SceneTab/GasSourceWidget.h>
#include <SceneTab/GravityWidget.h>
#include <SceneTab/HeaterWidget.h>
#include <SceneTab/ParticleAnimationWidget.h>
#include <SceneTab/ParticleFieldWidget.h>
#include <SceneTab/PropertiesWidgetFactory.h>
#include <SceneTab/SizeControlWidget.h>
#include <SceneTab/TypeMaskWidget.h>
#include <SceneTab/VisibilityControlWidget.h>
#include <ParticlesEditorCommonData.h>
#include <ParticlesEditorScene.h>

PropertiesWidgetFactory::PropertiesWidgetFactory(
                                        QVBoxLayout& widgetsLayout,
                                        ParticlesEditorCommonData& commonData) :
  PEVisitorT( widgetsLayout,
              commonData,
              &commonData.scene()->dataRoot().modificatorsGroup(),
              &commonData.animationPlayer()),
  _commonData(commonData)
{
}

void PropertiesWidgetFactory::visitBlockerObject(BlockerObject& object)
{
  PEVisitorT::visitBlockerObject(object);
  widgetsLayout().addWidget(new BlockerWidget(object, _commonData.undoStack()));
  widgetsLayout().addWidget(
                          new TypeMaskWidget(object, _commonData.undoStack()));
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

void PropertiesWidgetFactory::visitGasSource(GasSource& object)
{
  PEVisitorT::visitGasSource(object);
  widgetsLayout().addWidget(
                          new GasSourceWidget(object, _commonData.undoStack()));
}

void PropertiesWidgetFactory::visitGravityModificator(
                                                    GravityModificator& object)
{
  PEVisitorT::visitGravityModificator(object);
  widgetsLayout().addWidget(new GravityWidget(object, _commonData.undoStack()));
  widgetsLayout().addWidget(
                          new TypeMaskWidget(object, _commonData.undoStack()));
}

void PropertiesWidgetFactory::visitHeaterObject(HeaterObject& object)
{
  PEVisitorT::visitHeaterObject(object);
  widgetsLayout().addWidget(new HeaterWidget(object, _commonData.undoStack()));
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

void PropertiesWidgetFactory::visitSizeControlObject(SizeControlObject& object)
{
  PEVisitorT::visitSizeControlObject(object);
  widgetsLayout().addWidget(new SizeControlWidget(object,
                                                  _commonData.undoStack()));
  widgetsLayout().addWidget(
                          new TypeMaskWidget(object, _commonData.undoStack()));
}

void PropertiesWidgetFactory::visitVisibilityControlObject(
                                                VisibilityControlObject& object)
{
  PEVisitorT::visitVisibilityControlObject(object);
  widgetsLayout().addWidget(new VisibilityControlWidget(
                                                      object,
                                                      _commonData.undoStack()));
  widgetsLayout().addWidget(
                          new TypeMaskWidget(object, _commonData.undoStack()));
}
