#pragma once

#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>

#include <mtt/Core/EditCommands/SetPropertyCommand.h>
#include <mtt/Core/EditCommands/UndoStack.h>
#include <mtt/Core/Widgets/PropertiesWidgets/ReferenceWidget.h>
#include <mtt/Utilities/Abort.h>

template <typename ObjectClass>
class TexturePropertyWidget : public mtt::ReferenceWidget
{
public:
  using Getter = const QString& (ObjectClass::*)() const noexcept;
  using Setter = void (ObjectClass::*)(const QString&);

public:
  template<typename Signal>
  inline TexturePropertyWidget( ObjectClass& object,
                                Getter getter,
                                Setter setter,
                                Signal signal,
                                mtt::UndoStack& undoStack);
  TexturePropertyWidget(const TexturePropertyWidget&) = delete;
  TexturePropertyWidget& operator = (const TexturePropertyWidget&) = delete;
  virtual ~TexturePropertyWidget() noexcept = default;

protected:
  inline virtual void selectReference() noexcept override;
  inline virtual void resetReference() noexcept override;

private:
  inline void _updateWidget() noexcept;
  inline void _setFilename(const QString& filename) noexcept;

private:
  ObjectClass& _object;
  Getter _getter;
  Setter _setter;

  mtt::UndoStack& _undoStack;
};

template<typename ObjectClass>
template<typename Signal>
inline TexturePropertyWidget<ObjectClass>::
                            TexturePropertyWidget(ObjectClass& object,
                                                  Getter getter,
                                                  Setter setter,
                                                  Signal signal,
                                                  mtt::UndoStack& undoStack) :
  _object(object),
  _getter(getter),
  _setter(setter),
  _undoStack(undoStack)
{
  connect(&_object,
          signal,
          this,
          &TexturePropertyWidget::_updateWidget,
          Qt::DirectConnection);
  _updateWidget();
}

template<typename ObjectClass>
inline void TexturePropertyWidget<ObjectClass>::_updateWidget() noexcept
{
  try
  {
    QString fileName = (_object.*_getter)();
    if (fileName.isEmpty()) setNameLabel(tr("none"));
    else
    {
      QFileInfo fileInfo(fileName);
      setNameLabel(fileInfo.fileName());
    }
  }
  catch (...)
  {
    mtt::Abort("Vec3PropertyWidget::_updateWidget: unable to update.");
  }
}

template<typename ObjectClass>
inline void TexturePropertyWidget<ObjectClass>::selectReference() noexcept
{
  QString fileName = QFileDialog::getOpenFileName(
                                this,
                                tr("Texture"),
                                "",
                                tr("picture files(*.png *.jpg *.jpeg *.bmp)"));
  if(fileName.isEmpty()) return;
  _setFilename(fileName);
}

template< typename ObjectClass>
inline void TexturePropertyWidget<ObjectClass>::resetReference() noexcept
{
  _setFilename(QString());
}

template< typename ObjectClass>
inline void TexturePropertyWidget<ObjectClass>::
                                _setFilename(const QString& fileName) noexcept
{
  if((_object.*_getter)() == fileName) return;

  try
  {
    using Command = mtt::SetPropertyCommand<ObjectClass, QString, Setter>;
    std::unique_ptr<Command> command(new Command( _object,
                                                  _setter,
                                                  (_object.*_getter)(),
                                                  fileName));
    _undoStack.addAndMake(std::move(command));
  }
  catch(...)
  {
    mtt::Abort("TexturePropertyWidget::_setFilename: unable to update filename.");
  }
}