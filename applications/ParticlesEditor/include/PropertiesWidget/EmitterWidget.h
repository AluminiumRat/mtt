#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/EnumComboConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatRangeSpinConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/IntSpinConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/TimeRangeSpinConnection.h>

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
  std::unique_ptr<Ui::EmitterWidget> _ui;

  EmitterObject& _emitter;

  using FloatConnection = mtt::FloatSpinConnection<EmitterObject>;
  std::optional<FloatConnection> _intensityConnection;
  std::optional<FloatConnection> _sizeConnection;
  std::optional<FloatConnection> _angleConnection;

  using BoolConnection = mtt::BoolCheckboxConnection<EmitterObject>;
  std::optional<BoolConnection> _ignoreBrightnessConnection;
  std::optional<BoolConnection> _enabledConnection;

  using RangeConnection = mtt::FloatRangeSpinConnection<EmitterObject>;
  std::optional<RangeConnection> _speedConnection;
  std::optional<RangeConnection> _sizeRangeConnection;
  std::optional<RangeConnection> _rotationConnection;
  std::optional<RangeConnection> _rotationSpeedConnection;
  std::optional<RangeConnection> _opacityConnection;
  std::optional<RangeConnection> _brightnessConnection;
  std::optional<RangeConnection> _massConnection;
  std::optional<RangeConnection> _frictionFactorConnection;

  using Uint8Connection = mtt::IntSpinConnection<EmitterObject, uint8_t>;
  std::optional<Uint8Connection> _textureConnection;
  std::optional<Uint8Connection> _tileConnection;

  using LifetimeConnection = mtt::TimeRangeSpinConnection<EmitterObject>;
  std::optional<LifetimeConnection> _lifetimeConnection;

  using ShapeConnection = mtt::EnumComboConnection< EmitterObject,
                                                    EmitterObject::Shape>;
  std::optional<ShapeConnection> _shapeConnection;

  using DistributionConnection = mtt::EnumComboConnection<
                                                  EmitterObject,
                                                  EmitterObject::Distribution>;
  std::optional<DistributionConnection> _distributionConnection;
};