#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/EnumComboConnection.h>
#include <mtt/editorLib/Objects/FadingLightObject.h>

namespace Ui
{
  class FadingLightWidget;
}

namespace mtt
{
  class UndoStack;

  class FadingLightWidget : public QWidget
  {
  public:
    FadingLightWidget(FadingLightObject& object, UndoStack& undoStack);
    FadingLightWidget(const FadingLightWidget&) = delete;
    FadingLightWidget& operator = (const FadingLightWidget&) = delete;
    virtual ~FadingLightWidget() noexcept;

  private:
    std::unique_ptr<Ui::FadingLightWidget> _ui;

    using FadeTypeConnection = mtt::EnumComboConnection<
                                                      FadingLightObject,
                                                      clPipeline::LightingFade>;
    std::optional<FadeTypeConnection> _fadeTypeConnection;
  };
}