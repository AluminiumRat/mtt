#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/Bitfield32PropertyConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/EnumComboConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatRangeSpinConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/EmitterObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class EmitterWidget;
}

class EmitterWidget : public QWidget
{
public:
  EmitterWidget(EmitterObject& object, mtt::UndoStack& undoStack);
  EmitterWidget(const EmitterWidget&) = delete;
  EmitterWidget& operator = (const EmitterWidget&) = delete;
  virtual ~EmitterWidget() noexcept;

private:
  void emitParticles() noexcept;
  void step() noexcept;

private:
  std::unique_ptr<Ui::EmitterWidget> _ui;

  EmitterObject& _emitter;

  using FloatConnection = mtt::FloatSpinConnection<EmitterObject>;
  std::optional<FloatConnection> _intensityConnection;
  std::optional<FloatConnection> _sizeConnection;
  std::optional<FloatConnection> _angleConnection;

  using RangeConnection = mtt::FloatRangeSpinConnection<EmitterObject>;
  std::optional<RangeConnection> _speedConnection;

  using ShapeConnection = mtt::EnumComboConnection< EmitterObject,
                                                    EmitterObject::Shape>;
  std::optional<ShapeConnection> _shapeConnection;

  using DistributionConnection = mtt::EnumComboConnection<
                                                  EmitterObject,
                                                  EmitterObject::Distribution>;
  std::optional<DistributionConnection> _distributionConnection;

  using TypeMaskConnection = mtt::Bitfield32PropertyConnection<EmitterObject>;
  std::optional<TypeMaskConnection> _typemaskConnection;
};