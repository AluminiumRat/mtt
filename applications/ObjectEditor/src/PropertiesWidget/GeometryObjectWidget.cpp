#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>

#include <PropertiesWidget/GeometryObjectWidget.h>

#include <GeneratedFiles/ui_GeometryObjectWidget.h>


GeometryObjectWidget::GeometryObjectWidget( GeometryObject& object,
                                            mtt::Object& skeletonGroup,
                                            mtt::UndoStack& undoStack) :
  _ui(new Ui::GeometryObjectWidget)
{
  _ui->setupUi(this);

  using ReferenceWidget = mtt::ReferencePropertyWidget<
                                                  GeometryObject,
                                                  mtt::SkeletonObject,
                                                  &GeometryObject::skeletonRef>;

  ReferenceWidget* referenceWidget = new ReferenceWidget(
                                            object,
                                            &GeometryObject::skeletonRefChanged,
                                            undoStack,
                                            skeletonGroup);
  _ui->boneLayout->addWidget(referenceWidget, 3);
  adjustSize();
}

GeometryObjectWidget::~GeometryObjectWidget() noexcept = default;
