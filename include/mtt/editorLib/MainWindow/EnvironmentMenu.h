#pragma once

#include <QtWidgets/QMenu>

namespace mtt
{
  class EditorCommonData;

  class EnvironmentMenu : public QMenu
  {
    Q_OBJECT

  public:
    EnvironmentMenu(QWidget& window, EditorCommonData& commonData);
    EnvironmentMenu(const EnvironmentMenu&) = delete;
    EnvironmentMenu& operator = (const EnvironmentMenu&) = delete;
    virtual ~EnvironmentMenu() noexcept = default;

  private:
    void _loadEnvironment() noexcept;
    void _saveEnvironment() noexcept;
    void _saveEnvironmentAs() noexcept;
    void _saveEnvironmentToFile(const QString& file) noexcept;
    void _addAmbientLight() noexcept;
    template<typename LightType>
    void _addLight(const QString& name) noexcept;
    void _addDirectLight() noexcept;
    void _addSpotLight() noexcept;
    void _addPointLight() noexcept;
    void _addEnvironmentModel() noexcept;

  private:
    QWidget& _window;
    EditorCommonData& _commonData;
  };
}