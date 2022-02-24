#include <QtWidgets/QVBoxLayout>

#include <SceneTab/ParticleFieldWidget.h>
#include <SceneTab/PropertiesWidgetFactory.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

PropertiesWidgetFactory::PropertiesWidgetFactory(
                                                QVBoxLayout& widgetsLayout,
                                                EditorCommonData& commonData) :
  PEVisitorT( widgetsLayout,
              commonData,
              &commonData.scene()->root().skeletonGroup(),
              &commonData.animationPlayer()),
  _commonData(commonData)
{
}

void PropertiesWidgetFactory::visitParticleField(ParticleField& object)
{
  PEVisitorT::visitParticleField(object);
  widgetsLayout().addWidget(new ParticleFieldWidget(object,
                                                    _commonData.undoStack()));
}
