#pragma once

#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>

#include <mtt/Application/EditCommands/SetPropertyCommand.h>
#include <mtt/Application/EditCommands/UndoStack.h>
#include <mtt/Application/Widgets/PropertiesWidgets/ReferenceWidget.h>
#include <mtt/Utilities/Abort.h>

template <typename ObjectClass>
class FilenamePropertyWidget : public mtt::ReferenceWidget
{
public:
  using Getter = const QString& (ObjectClass::*)() const noexcept;
  using Setter = void (ObjectClass::*)(const QString&);

public:
  template<typename Signal>
  inline FilenamePropertyWidget(ObjectClass& object,
                                Getter getter,
                                Setter setter,
                                Signal signal,
                                const QString& fileFilter,
                                mtt::UndoStack& undoStack);
  FilenamePropertyWidget(const FilenamePropertyWidget&) = delete;
  FilenamePropertyWidget& operator = (const FilenamePropertyWidget&) = delete;
  virtual ~FilenamePropertyWidget() noexcept = default;

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

  QString _fileFilter;

  mtt::UndoStack& _undoStack;
};

template<typename ObjectClass>
template<typename Signal>
inline FilenamePropertyWidget<ObjectClass>::
                          FilenamePropertyWidget( ObjectClass& object,
                                                  Getter getter,
                                                  Setter setter,
                                                  Signal signal,
                                                  const QString& fileFilter,
                                                  mtt::UndoStack& undoStack) :
  _object(object),
  _getter(getter),
  _setter(setter),
  _fileFilter(fileFilter),
  _undoStack(undoStack)
{
  connect(&_object,
          signal,
          this,
          &FilenamePropertyWidget::_updateWidget,
          Qt::DirectConnection);
  _updateWidget();
}

template<typename ObjectClass>
inline void FilenamePropertyWidget<ObjectClass>::_updateWidget() noexcept
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
inline void FilenamePropertyWidget<ObjectClass>::selectReference() noexcept
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Select file"),
                                                  "",
                                                  _fileFilter);
  if(fileName.isEmpty()) return;
  _setFilename(fileName);
}

template< typename ObjectClass>
inline void FilenamePropertyWidget<ObjectClass>::resetReference() noexcept
{
  _setFilename(QString());
}

template< typename ObjectClass>
inline void FilenamePropertyWidget<ObjectClass>::
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
    mtt::Abort("FilenamePropertyWidget::_setFilename: unable to update filename.");
  }
}