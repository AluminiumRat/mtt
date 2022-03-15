#include <mtt/application/Widgets/PropertiesWidgets/Bitfield32Widget.h>

#include <SceneTab/TypeMaskWidget.h>

#include <GeneratedFiles/ui_TypeMaskWidget.h>

TypeMaskWidget::TypeMaskWidget( ModificatorObject& object,
                                mtt::UndoStack& undoStack) :
  _ui(new Ui::TypeMaskWidget)
{
  _ui->setupUi(this);

  std::unique_ptr<mtt::Bitfield32Widget> typeMaskWidget(
                                                    new mtt::Bitfield32Widget);
  _typemaskConnection.emplace(*typeMaskWidget,
                              object,
                              &ModificatorObject::typeMask,
                              &ModificatorObject::setTypeMask,
                              &ModificatorObject::typeMaskChanged,
                              undoStack);
  _ui->typeMaskLayout->addWidget(typeMaskWidget.release(), 3);

  adjustSize();
};

TypeMaskWidget::~TypeMaskWidget() noexcept = default;
