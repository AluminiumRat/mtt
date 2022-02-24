#include <mtt/application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h>

#include <SceneTab/ParticleFieldWidget.h>

#include <GeneratedFiles/ui_ParticleFieldWidget.h>

ParticleFieldWidget::ParticleFieldWidget( ParticleField& object,
                                          mtt::UndoStack& undoStack) :
  _ui(new Ui::ParticleFieldWidget)
{
  _ui->setupUi(this);

  mtt::Vec3PropertyWidget<ParticleField>* sizeWidget =
                      new mtt::Vec3PropertyWidget<ParticleField>(
                                                    object,
                                                    &ParticleField::size,
                                                    &ParticleField::setSize,
                                                    &ParticleField::sizeChanged,
                                                    undoStack);
  sizeWidget->xSpin().setMinimum(0.1);
  sizeWidget->xSpin().setSingleStep(.1);
  sizeWidget->ySpin().setMinimum(0.1);
  sizeWidget->ySpin().setSingleStep(.1);
  sizeWidget->zSpin().setMinimum(0.1);
  sizeWidget->zSpin().setSingleStep(.1);
  _ui->sizeLayout->addWidget(sizeWidget, 3);

  adjustSize();
}

ParticleFieldWidget::~ParticleFieldWidget() noexcept = default;