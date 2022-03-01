#include <random>

#include <mtt/application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h>

#include <SceneTab/ParticleFieldWidget.h>

#include <GeneratedFiles/ui_ParticleFieldWidget.h>

ParticleFieldWidget::ParticleFieldWidget( ParticleField& object,
                                          mtt::UndoStack& undoStack) :
  _ui(new Ui::ParticleFieldWidget),
  _field(object)
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

  connect(_ui->clearButton,
          &QPushButton::pressed,
          this,
          &ParticleFieldWidget::_clear,
          Qt::DirectConnection);

  connect(_ui->emitButton,
          &QPushButton::pressed,
          this,
          &ParticleFieldWidget::_emitParticles,
          Qt::DirectConnection);

  connect(_ui->stepButton,
          &QPushButton::pressed,
          this,
          &ParticleFieldWidget::_stepSimulation,
          Qt::DirectConnection);

  adjustSize();
}

ParticleFieldWidget::~ParticleFieldWidget() noexcept = default;

void ParticleFieldWidget::_clear() noexcept
{
  _field.clear();
}

void ParticleFieldWidget::_emitParticles() noexcept
{
  std::default_random_engine randomEngine;
  std::uniform_real_distribution<float> symmetricalDistribution(-1.f, 1.f);
  std::uniform_real_distribution<float> displacedDistribution(0.f, 1.f);
  std::uniform_int_distribution<int> timeDistribution(300, 5000);

  for (size_t i = 0; i < 20; i++)
  {
    ParticleField::ParticleData newParticle;
    newParticle.typeIndex = 0;
    newParticle.position = glm::vec3( symmetricalDistribution(randomEngine),
                                      symmetricalDistribution(randomEngine),
                                      symmetricalDistribution(randomEngine));
    newParticle.position *= _field.size() / 2.f;
    newParticle.speed = glm::vec3(symmetricalDistribution(randomEngine),
                                  symmetricalDistribution(randomEngine),
                                  symmetricalDistribution(randomEngine));
    newParticle.size = displacedDistribution(randomEngine) + .1f;
    newParticle.rotation = symmetricalDistribution(randomEngine);
    newParticle.rotationSpeed = symmetricalDistribution(randomEngine);
    newParticle.color = glm::vec3(displacedDistribution(randomEngine),
                                  displacedDistribution(randomEngine),
                                  displacedDistribution(randomEngine));
    newParticle.brightness = .5f + .5f * displacedDistribution(randomEngine);
    newParticle.transparency = 1.f - newParticle.brightness;
    newParticle.textureIndex = 0;
    newParticle.currentTime = ParticleField::TimeType(0);
    newParticle.maxTime =
                        ParticleField::TimeType(timeDistribution(randomEngine));

    _field.addParticle(newParticle);
  }
}

void ParticleFieldWidget::_stepSimulation() noexcept
{
  _field.simulationStep(std::chrono::duration_cast<ParticleField::TimeType>(
                                              std::chrono::milliseconds(100)));
}
