#include <memory>

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

#include <mtt/application/Widgets/PropertiesWidgets/NamePropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>
#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/PropertiesWidget/AmbientLightWidget.h>
#include <mtt/editorLib/PropertiesWidget/AnimationObjectWidget.h>
#include <mtt/editorLib/PropertiesWidget/AnimationTrackWidget.h>
#include <mtt/editorLib/PropertiesWidget/BackgroundWidget.h>
#include <mtt/editorLib/PropertiesWidget/CubemapWidget.h>
#include <mtt/editorLib/PropertiesWidget/DirectLightWidget.h>
#include <mtt/editorLib/PropertiesWidget/EnvironmentModelWidget.h>
#include <mtt/editorLib/PropertiesWidget/LightWidget.h>
#include <mtt/editorLib/PropertiesWidget/MovableObjectWidget.h>
#include <mtt/editorLib/PropertiesWidget/PositionAnimatorWidget.h>
#include <mtt/editorLib/PropertiesWidget/PropertiesWidgetFactory.h>
#include <mtt/editorLib/PropertiesWidget/RotatableObjectWidget.h>
#include <mtt/editorLib/PropertiesWidget/ScalableObjectWidget.h>
#include <mtt/editorLib/PropertiesWidget/ShadowsWidget.h>
#include <mtt/editorLib/PropertiesWidget/SpotLightWidget.h>
#include <mtt/editorLib/PropertiesWidget/VisiblePropertyWidget.h>

using namespace mtt;

PropertiesWidgetFactory::PropertiesWidgetFactory(
                                            QVBoxLayout& widgetsLayout,
                                            CommonEditData& commonEditData,
                                            Object* skeletonSelectArea,
                                            AnimationPlayer* animationPlayer) :
  _widgetsLayout(widgetsLayout),
  _commonEditData(commonEditData),
  _skeletonSelectArea(skeletonSelectArea),
  _animationPlayer(animationPlayer)
{
}

void PropertiesWidgetFactory::visitAmbientLightObject(
                                                    AmbientLightObject& object)
{
  CEVisitor::visitAmbientLightObject(object);
  _widgetsLayout.addWidget(
                        new AmbientLightWidget( object,
                                                _commonEditData.undoStack()));
}

void PropertiesWidgetFactory::visitAnimationObject(AnimationObject& object)
{
  CEVisitor::visitAnimationObject(object);

  if(_animationPlayer != nullptr)
  {
    _widgetsLayout.addWidget(
                          new AnimationObjectWidget(object, *_animationPlayer));
  }
}

void PropertiesWidgetFactory::visitAnimationTrack(AnimationTrack& object)
{
  CEVisitor::visitAnimationTrack(object);
  if(_skeletonSelectArea != nullptr)
  {
    _widgetsLayout.addWidget(
                    new AnimationTrackWidget( object,
                                              *_skeletonSelectArea,
                                              _commonEditData.undoStack()));
  }
}

void PropertiesWidgetFactory::visitBackgroundObject(BackgroundObject& object)
{
  CEVisitor::visitBackgroundObject(object);

  _widgetsLayout.addWidget(
                  new BackgroundWidget( object, _commonEditData.undoStack()));
}

void PropertiesWidgetFactory::visitCubemapObject(CubemapObject& object)
{
  CEVisitor::visitCubemapObject(object);

  _widgetsLayout.addWidget(new CubemapWidget( object,
                                              _commonEditData.undoStack()));
}


void PropertiesWidgetFactory::visitDirectLightObject(
                                                DirectLightObject& object)
{
  CEVisitor::visitDirectLightObject(object);

  _widgetsLayout.addWidget(
                  new DirectLightWidget(object, _commonEditData.undoStack()));
}

void PropertiesWidgetFactory::visitDisplayedObject(DisplayedObject& object)
{
  CEVisitor::visitDisplayedObject(object);

  _widgetsLayout.addWidget(
              new VisiblePropertyWidget(object, _commonEditData.undoStack()));
}

void PropertiesWidgetFactory::visitEnvironmentModel(EnvironmentModel& object)
{
  CEVisitor::visitEnvironmentModel(object);

  _widgetsLayout.addWidget(
            new EnvironmentModelWidget(object, _commonEditData.undoStack()));
}

void PropertiesWidgetFactory::visitObject(Object& object)
{
  CEVisitor::visitObject(object);
  _widgetsLayout.addWidget(
                        new NamePropertyWidget( object,
                                                _commonEditData.undoStack()));
}

void PropertiesWidgetFactory::visitPositionAnimator(PositionAnimator& object)
{
  CEVisitor::visitPositionAnimator(object);
  if (_skeletonSelectArea != nullptr)
  {
    _widgetsLayout.addWidget(
                      new PositionAnimatorWidget( object,
                                                  *skeletonSelectArea(),
                                                  _commonEditData.undoStack()));
  }
}

void PropertiesWidgetFactory::visitLightObject(LightObject& object)
{
  CEVisitor::visitLightObject(object);

  _widgetsLayout.addWidget(
                        new LightWidget(object, _commonEditData.undoStack()));
}

void PropertiesWidgetFactory::visitMovableObject(MovableObject& object)
{
  visitObject3D(object);

  _widgetsLayout.addWidget(
                new MovableObjectWidget(object, _commonEditData.undoStack()));
}

void PropertiesWidgetFactory::visitPointLightObject(PointLightObject& object)
{
  CEVisitor::visitPointLightObject(object);

  _widgetsLayout.addWidget( new ShadowsWidget(object,
                                              180.f / glm::pi<float>(),
                                              20.f,
                                              _commonEditData.undoStack()));

  std::unique_ptr<QLabel> filterLabel(new QLabel);
  filterLabel->setText(QObject::tr("Filter:"));
  _widgetsLayout.addWidget(filterLabel.release());
  _widgetsLayout.addWidget(new CubemapWidget( object.filterCubemap(),
                                              _commonEditData.undoStack()));
}

void PropertiesWidgetFactory::visitRotatableObject(RotatableObject& object)
{
  CEVisitor::visitRotatableObject(object);

  _widgetsLayout.addWidget(
              new RotatableObjectWidget(object, _commonEditData.undoStack()));
}

void PropertiesWidgetFactory::visitScalableObject(ScalableObject& object)
{
  CEVisitor::visitScalableObject(object);

  _widgetsLayout.addWidget(
              new ScalableObjectWidget(object, _commonEditData.undoStack()));
}

void PropertiesWidgetFactory::visitSpotLightObject(SpotLightObject& object)
{
  CEVisitor::visitSpotLightObject(object);

  _widgetsLayout.addWidget(
                      new SpotLightWidget(object, _commonEditData.undoStack()));
}
