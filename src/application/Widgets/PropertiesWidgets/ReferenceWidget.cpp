#include <mtt/application/Widgets/PropertiesWidgets/ReferenceWidget.h>

#include <GeneratedFiles/ui_ReferencePropertyWidget.h>

using namespace mtt;

ReferenceWidget::ReferenceWidget() :
  _ui(new Ui::ReferencePropertyWidget)
{
  _ui->setupUi(this);
  connect(_ui->changeButton,
          &QPushButton::pressed,
          this,
          &ReferenceWidget::selectReference,
          Qt::DirectConnection);

  connect(_ui->removeButton,
          &QPushButton::pressed,
          this,
          &ReferenceWidget::resetReference,
          Qt::DirectConnection);
}

ReferenceWidget::~ReferenceWidget() noexcept
{
  delete _ui;
}

void ReferenceWidget::setNameLabel(const QString& newName) noexcept
{
  _ui->nameLabel->setText(newName);
}
