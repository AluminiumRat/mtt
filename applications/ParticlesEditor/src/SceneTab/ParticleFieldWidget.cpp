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

void ParticleFieldWidget::_stepSimulation() noexcept
{
  _field.simulationStep(mtt::TimeT(0),
                        std::chrono::duration_cast<mtt::TimeT>(
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
  _undoStack.addAndMake(
                    mtt::makeSetPropertyCommand(_field,
                                                &ParticleField::textureFiles,
                                                &ParticleField::setTextureFiles,
                                                files));
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
