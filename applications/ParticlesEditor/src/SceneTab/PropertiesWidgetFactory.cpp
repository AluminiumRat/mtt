#include <QtWidgets/QVBoxLayout>

#include <SceneTab/EmitterWidget.h>
#include <SceneTab/ParticleFieldWidget.h>
#include <SceneTab/PropertiesWidgetFactory.h>
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
}

void PropertiesWidgetFactory::visitParticleField(ParticleField& object)
{
  PEVisitorT::visitParticleField(object);
  widgetsLayout().addWidget(new ParticleFieldWidget(object,
                                                    _commonData.undoStack()));
}
