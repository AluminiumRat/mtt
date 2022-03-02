#include <exception>
#include <random>

#include <QtCore/QFileInfo>
#include <QtWidgets/QFileDialog>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h>
#include <mtt/utilities/Log.h>

#include <SceneTab/ParticleFieldWidget.h>

#include <GeneratedFiles/ui_ParticleFieldWidget.h>

ParticleFieldWidget::ParticleFieldWidget( ParticleField& object,
                                          mtt::UndoStack& undoStack) :
  _ui(new Ui::ParticleFieldWidget),
  _field(object),
  _undoStack(undoStack)
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

  connect(&_field,
          &ParticleField::textureFilesChanged,
          this,
          &ParticleFieldWidget::_updateTextures,
          Qt::DirectConnection);
  _updateTextures();

  connect(_ui->addTextureButton,
          &QPushButton::pressed,
          this,
          &ParticleFieldWidget::_addTexture,
          Qt::DirectConnection);

  connect(_ui->textureRemoveButton,
          &QPushButton::pressed,
          this,
          &ParticleFieldWidget::_removeTexture,
          Qt::DirectConnection);

  connect(_ui->textureUpButton,
          &QPushButton::pressed,
          this,
          &ParticleFieldWidget::_moveTextureUp,
          Qt::DirectConnection);

  connect(_ui->textureDownButton,
          &QPushButton::pressed,
          this,
          &ParticleFieldWidget::_moveTextureDown,
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
    newParticle.opacity = newParticle.brightness;
    newParticle.textureIndex = 0;
    if(i > 15) newParticle.textureIndex = 1;
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

void ParticleFieldWidget::_updateTextures() noexcept
{
  _ui->texturesList->clear();
  for (const QString& textureName : _field.textureFiles())
  {
    QFileInfo fileInfo(textureName);
    _ui->texturesList->addItem(fileInfo.fileName());
  }
}

void ParticleFieldWidget::_setTextureFiles(const std::vector<QString>& files)
{
  using Command = mtt::SetPropertyCommand<
                          ParticleField,
                          std::vector<QString>,
                          void(ParticleField::*)(const std::vector<QString>&)>;
  std::unique_ptr<Command> command(new Command( _field,
                                                &ParticleField::setTextureFiles,
                                                _field.textureFiles(),
                                                files));
  _undoStack.addAndMake(std::move(command));
}

void ParticleFieldWidget::_addTexture() noexcept
{
  try
  {
    QString fileName = QFileDialog::getOpenFileName(
                              this,
                              tr("Select file"),
                              "",
                              tr("picture files(*.png *.jpg *.jpeg *.bmp)"));
    if(fileName.isEmpty()) return;

    std::vector<QString> textureFiles = _field.textureFiles();
    textureFiles.push_back(fileName);
    _setTextureFiles(textureFiles);
  }
  catch (std::exception& error)
  {
    mtt::Log() << "ParticleFieldWidget::_addTexture: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "ParticleFieldWidget::_addTexture: unknown error";
  }
}

void ParticleFieldWidget::_removeTexture() noexcept
{
  try
  {
    int textureIndex = _ui->texturesList->currentRow();
    if(textureIndex < 0) return;
    std::vector<QString> textureFiles = _field.textureFiles();
    textureFiles.erase(textureFiles.begin() + textureIndex);
    _setTextureFiles(textureFiles);
  }
  catch (std::exception& error)
  {
    mtt::Log() << "ParticleFieldWidget::_removeTexture: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "ParticleFieldWidget::_removeTexture: unknown error";
  }
}

void ParticleFieldWidget::_moveTextureUp() noexcept
{
  try
  {
    int textureIndex = _ui->texturesList->currentRow();
    if(textureIndex < 1) return;
    std::vector<QString> textureFiles = _field.textureFiles();
    std::swap(textureFiles[textureIndex], textureFiles[textureIndex-1]);
    _setTextureFiles(textureFiles);
    _ui->texturesList->setCurrentRow(textureIndex - 1);
  }
  catch (std::exception& error)
  {
    mtt::Log() << "ParticleFieldWidget::_moveTextureUp: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "ParticleFieldWidget::_moveTextureUp: unknown error";
  }
}

void ParticleFieldWidget::_moveTextureDown() noexcept
{
  try
  {
    int textureIndex = _ui->texturesList->currentRow();
    if(textureIndex < 0) return;

    std::vector<QString> textureFiles = _field.textureFiles();
    if (textureIndex >= textureFiles.size() - 1) return;

    std::swap(textureFiles[textureIndex], textureFiles[textureIndex + 1]);
    _setTextureFiles(textureFiles);
    _ui->texturesList->setCurrentRow(textureIndex + 1);
  }
  catch (std::exception& error)
  {
    mtt::Log() << "ParticleFieldWidget::_moveTextureDown: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "ParticleFieldWidget::_moveTextureDown: unknown error";
  }
}
