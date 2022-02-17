#include <stdexcept>

#include <QtCore/QFileInfo>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/editorLib/Objects/CubemapObject.h>
#include <mtt/editorLib/SceneTab/CubemapWidget.h>
#include <mtt/utilities/Log.h>

#include <GeneratedFiles/ui_CubemapWidget.h>

using namespace mtt;

CubemapWidget::CubemapWidget( CubemapObject& object, UndoStack& undoStack) :
  _ui(new Ui::CubemapWidget),
  _object(object),
  _undoStack(undoStack)
{
  _ui->setupUi(this);

  connect(&object,
          &CubemapObject::texturesChanged,
          this,
          &CubemapWidget::_updateWidget,
          Qt::DirectConnection);
  _updateWidget();

  connect(_ui->selectButton,
          &QPushButton::pressed,
          this,
          &CubemapWidget::_selectTextures,
          Qt::DirectConnection);

  connect(_ui->resetButton,
          &QPushButton::pressed,
          this,
          &CubemapWidget::_resetTextures,
          Qt::DirectConnection);
}

CubemapWidget::~CubemapWidget() noexcept
{
  delete _ui;
}

QString CubemapWidget::prepareName(const QString& fullName) noexcept
{
  if(fullName.isEmpty()) return tr("none");
  QFileInfo fileInfo(fullName);
  return fileInfo.fileName();
}

void CubemapWidget::_updateWidget() noexcept
{
  QString sideTexture = _object.sideTexture(CubemapObject::SIDE_X_POSITIVE);
  _ui->pXFileLabel->setText(prepareName(sideTexture));

  sideTexture = _object.sideTexture(CubemapObject::SIDE_X_NEGATIVE);
  _ui->nXFileLabel->setText(prepareName(sideTexture));

  sideTexture = _object.sideTexture(CubemapObject::SIDE_Y_POSITIVE);
  _ui->pYFileLabel->setText(prepareName(sideTexture));

  sideTexture = _object.sideTexture(CubemapObject::SIDE_Y_NEGATIVE);
  _ui->nYFileLabel->setText(prepareName(sideTexture));

  sideTexture = _object.sideTexture(CubemapObject::SIDE_Z_NEGATIVE);
  _ui->nZFileLabel->setText(prepareName(sideTexture));

  sideTexture = _object.sideTexture(CubemapObject::SIDE_Z_POSITIVE);
  _ui->pZFileLabel->setText(prepareName(sideTexture));
}

void CubemapWidget::_resetTextures() noexcept
{
  try
  {
    QStringList textures;
    for(size_t i = 0; i < 6; i++) textures.push_back("");
    _setTextures(textures);
  }
  catch(...)
  {
    QMessageBox::critical(this, tr("Unable to reset cubemap."), tr("Unable to reset cubemap."));
  }
}

void CubemapWidget::_selectTextures() noexcept
{
  try
  {
    QStringList fileNames = QFileDialog::getOpenFileNames(
                                  this,
                                  tr("Texture"),
                                  "",
                                  tr("picture files(*.png *.jpg *.jpeg *.bmp)"));
    if(fileNames.size() == 0) return;
    if(fileNames.size() != 6)
    {
      QMessageBox::warning(this, tr("Unable to set textures"), tr("You have to select 6 files."));
      return;
    }

    _setTextures(fileNames);
  }
  catch (...)
  {
    QMessageBox::critical(this, tr("Unable to change cubemap."), tr("Unable to change cubemap."));
  }
}

void CubemapWidget::_setTextures(const QStringList& texturesList)
{
  try
  {
    CubemapObject::Textures textures;
    for(int i = 0; i < 6; i++) textures[i] = texturesList[i];

    if(textures == _object.textures()) return;

    using Command = SetPropertyCommand< CubemapObject,
                                        CubemapObject::Textures,
                                        void(CubemapObject::*)(
                                              const CubemapObject::Textures&)>;
    std::unique_ptr<Command> command( new Command(_object,
                                                  &CubemapObject::setTextures,
                                                  _object.textures(),
                                                  textures));
    _undoStack.addAndMake(std::move(command));
  }
  catch(std::exception& error)
  {
    Log() << "CubemapWidget::_setTextures: " << error.what();
    throw;
  }
}
