#include <stdexcept>

#include <QtWidgets/QVBoxLayout>

#include <mtt/editorLib/PropertiesWidget/PropertiesWidget.h>
#include <mtt/editorLib/EditorCommonData.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

PropertiesWidget::PropertiesWidget(EditorCommonData& commonData) :
  _commonData(commonData),
  _widgetsFrame(nullptr),
  _widgetsLayout(nullptr)
{
  setFrameShape(QFrame::NoFrame);
  setWidgetResizable(true);

  std::unique_ptr<QWidget> widgetsFrame(new QWidget);
  _widgetsFrame = widgetsFrame.get();
  setWidget(widgetsFrame.release());

  std::unique_ptr<QVBoxLayout> frameLayout(new QVBoxLayout);

  std::unique_ptr<QVBoxLayout> widgetsLayout(new QVBoxLayout);
  _widgetsLayout = widgetsLayout.get();

  frameLayout->addLayout(widgetsLayout.release());
  frameLayout->addStretch();

  _widgetsFrame->setLayout(frameLayout.release());

  connect(&_commonData,
          &EditorCommonData::selectedObjectsChanged,
          this,
          &PropertiesWidget::_updatePropertiesWidgets,
          Qt::DirectConnection);
  _updatePropertiesWidgets();
}

std::unique_ptr<PropertiesWidgetFactory>
              PropertiesWidget::createWidgetsFactory(QVBoxLayout& widgetsLayout)
{
  return std::make_unique<PropertiesWidgetFactory>( widgetsLayout,
                                                    _commonData,
                                                    nullptr,
                                                    nullptr);
}

void PropertiesWidget::_updatePropertiesWidgets() noexcept
{
  _clearPropertiesWidgets();

  if(_commonData.selectedObjects().size() != 1) return;
  if(_commonData.scene() == nullptr) Abort("PropertiesWidget::_updatePropertiesWidgets: scene is null");

  try
  {
    std::unique_ptr<PropertiesWidgetFactory> widgetsFactory =
                                          createWidgetsFactory(*_widgetsLayout);
    if(widgetsFactory == nullptr) Abort("PropertiesWidget::_updatePropertiesWidgets: widgets factory is null.");
    widgetsFactory->process(*_commonData.selectedObjects()[0]);
    _widgetsFrame->adjustSize();
  }
  catch (std::exception& error)
  {
    Log() << "PropertiesWidget::_updatePropertiesWidgets: " << error.what();
    _clearPropertiesWidgets();
  }
  catch (...)
  {
    Log() << "PropertiesWidget::_updatePropertiesWidgets: unknown error";
    _clearPropertiesWidgets();
  }
}

void PropertiesWidget::_clearPropertiesWidgets() noexcept
{
  try
  {
    QList<QWidget*> childWidgets =
              _widgetsFrame->findChildren<QWidget*>(QString(),
                                                    Qt::FindDirectChildrenOnly);
    for (QWidget* child : childWidgets) delete child;
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("PropertiesWidget::_clearPropertiesWidgets: unable to clear widgets layout");
  }
  catch (...)
  {
    Abort("PropertiesWidget::_clearPropertiesWidgets: unknown error");
  }
}
