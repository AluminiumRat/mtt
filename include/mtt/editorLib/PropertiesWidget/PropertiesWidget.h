#pragma once

#include <QtWidgets/QScrollArea>

#include <mtt/editorLib/PropertiesWidget/PropertiesWidgetFactory.h>

class QVBoxLayout;

namespace mtt
{
  class EditorCommonData;

  class PropertiesWidget : public QScrollArea
  {
    Q_OBJECT

  public:
    explicit PropertiesWidget(EditorCommonData& commonData);
    PropertiesWidget(const PropertiesWidget&) = delete;
    PropertiesWidget& operator = (const PropertiesWidget&) = delete;
    virtual ~PropertiesWidget() noexcept = default;

  protected:
    inline EditorCommonData& commonData() const noexcept;

    /// The return value should not be nullptr
    virtual std::unique_ptr<PropertiesWidgetFactory> createWidgetsFactory(
                                                    QVBoxLayout& widgetsLayout);

  private:
    void _updatePropertiesWidgets() noexcept;
    void _clearPropertiesWidgets() noexcept;

  private:
    EditorCommonData& _commonData;
    QWidget* _widgetsFrame;
    QVBoxLayout* _widgetsLayout;
  };

  inline EditorCommonData& PropertiesWidget::commonData() const noexcept
  {
    return _commonData;
  }
}