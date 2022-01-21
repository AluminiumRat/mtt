#pragma once

#include <QtWidgets/QWidget>

namespace Ui
{
  class ReferencePropertyWidget;
}

namespace mtt
{
  class ReferenceWidget : public QWidget
  {
  public:
    ReferenceWidget();
    ReferenceWidget(const ReferenceWidget&) = delete;
    ReferenceWidget& operator = (const ReferenceWidget&) = delete;
    virtual ~ReferenceWidget() noexcept;
  
  protected:
    void setNameLabel(const QString& newName) noexcept;
    virtual void selectReference() noexcept = 0;
    virtual void resetReference() noexcept = 0;

  private:
    Ui::ReferencePropertyWidget* _ui;
  };
}