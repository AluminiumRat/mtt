#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/ParticleField.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class ParticleFieldWidget;
}

class ParticleFieldWidget : public QWidget
{
public:
  ParticleFieldWidget(ParticleField& object, mtt::UndoStack& undoStack);
  ParticleFieldWidget(const ParticleFieldWidget&) = delete;
  ParticleFieldWidget& operator = (const ParticleFieldWidget&) = delete;
  virtual ~ParticleFieldWidget() noexcept;

private:
  void _clear() noexcept;
  void _updateTextures() noexcept;
  void _updateExtentSpin() noexcept;
  void _setExtentValue() noexcept;
  void _setTextureFiles(const ParticleTextureDescriptions& files);
  void _addTexture() noexcept;
  void _removeTexture() noexcept;
  void _moveTextureUp() noexcept;
  void _moveTextureDown() noexcept;

private:
  std::unique_ptr<Ui::ParticleFieldWidget> _ui;
  ParticleField& _field;
  mtt::UndoStack& _undoStack;
  bool _skipUpdate;

  using FloatConnection = mtt::FloatSpinConnection<ParticleField>;
  std::optional<FloatConnection> _lodMppxConnection;
  std::optional<FloatConnection> _lodSmoothingConnection;
};