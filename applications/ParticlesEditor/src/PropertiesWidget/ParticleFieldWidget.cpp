#include <exception>
#include <random>

#include <QtCore/QFileInfo>
#include <QtWidgets/QFileDialog>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/ScopedSetter.h>

#include <PropertiesWidget/ParticleFieldWidget.h>

#include <GeneratedFiles/ui_ParticleFieldWidget.h>

ParticleFieldWidget::ParticleFieldWidget( ParticleField& object,
                                          mtt::UndoStack& undoStack) :
  _ui(new Ui::ParticleFieldWidget),
  _field(object),
  _undoStack(undoStack),
  _skipUpdate(false)
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

  connect(&_field,
          &ParticleField::textureDescriptionsChanged,
          this,
          &ParticleFieldWidget::_updateTextures,
          Qt::DirectConnection);
  _updateTextures();

  connect(_ui->texturesList,
          &QListWidget::currentRowChanged,
          this,
          &ParticleFieldWidget::_updateExtentSpin,
          Qt::DirectConnection);

  connect(_ui->extentSpin,
          QOverload<int>::of(&QSpinBox::valueChanged),
          this,
          &ParticleFieldWidget::_setExtentValue,
          Qt::DirectConnection);

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

  _lodMppxConnection.emplace( *_ui->lodMppxSpin,
                              object,
                              &ParticleField::lodMppx,
                              &ParticleField::setLodMppx,
                              &ParticleField::lodMppxChanged,
                              undoStack);

  _lodSmoothingConnection.emplace(*_ui->lodSmoothingSpin,
                                  object,
                                  &ParticleField::lodSmoothing,
                                  &ParticleField::setLodSmoothing,
                                  &ParticleField::lodSmoothingChanged,
                                  undoStack);
}

ParticleFieldWidget::~ParticleFieldWidget() noexcept = default;

void ParticleFieldWidget::_clear() noexcept
{
  _field.clear();
}

void ParticleFieldWidget::_updateTextures() noexcept
{
  if (_skipUpdate) return;

  _ui->texturesList->clear();
  for (const ParticleTextureDescription& texture : _field.textureDescriptions())
  {
    QFileInfo fileInfo(texture.filename);
    _ui->texturesList->addItem(fileInfo.fileName());
  }
}

void ParticleFieldWidget::_updateExtentSpin() noexcept
{
  mtt::ScopedSetter<bool> lock(_skipUpdate, true);

  int textureIndex = _ui->texturesList->currentRow();
  if (textureIndex < 0)
  {
    _ui->extentSpin->setValue(1);
    _ui->extentSpin->setEnabled(false);
  }
  else
  {
    _ui->extentSpin->setValue(
                            _field.textureDescriptions()[textureIndex].extent);
    _ui->extentSpin->setEnabled(true);
  }
}

void ParticleFieldWidget::_setExtentValue() noexcept
{
  if(_skipUpdate) return;
  mtt::ScopedSetter<bool> lock(_skipUpdate, true);

  int textureIndex = _ui->texturesList->currentRow();
  if (textureIndex < 0) return;
  ParticleTextureDescriptions textureFiles = _field.textureDescriptions();
  textureFiles[textureIndex].extent = uint8_t(_ui->extentSpin->value());
  _setTextureFiles(textureFiles);
}

void ParticleFieldWidget::_setTextureFiles(
                                      const ParticleTextureDescriptions& files)
{
  _undoStack.addAndMake(
            mtt::makeSetPropertyCommand(_field,
                                        &ParticleField::textureDescriptions,
                                        &ParticleField::setTextureDescriptions,
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

    ParticleTextureDescription newDescription;
    newDescription.filename = fileName;
    newDescription.extent = 1;

    ParticleTextureDescriptions textureFiles = _field.textureDescriptions();
    textureFiles.push_back(newDescription);
    _setTextureFiles(textureFiles);
    _ui->texturesList->setCurrentRow(textureFiles.size() - 1);
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
    ParticleTextureDescriptions textureFiles = _field.textureDescriptions();
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
    ParticleTextureDescriptions textureFiles = _field.textureDescriptions();
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

    ParticleTextureDescriptions textureFiles = _field.textureDescriptions();
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
