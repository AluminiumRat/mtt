#pragma once

#include <memory>

#include <mtt/application/Scene/Scene.h>
#include <mtt/editorLib/Objects/EnvironmentRootObject.h>

namespace mtt
{
  class EditorScene : public mtt::Scene
  {
    Q_OBJECT

  public:
    /// dataRoot and environmentRoot must not be nullptr
    EditorScene(std::unique_ptr<Object> dataRoot,
                std::unique_ptr<EnvironmentRootObject> environmentRoot);
    EditorScene(const EditorScene&) = delete;
    EditorScene& operator = (const EditorScene&) = delete;
    virtual ~EditorScene() noexcept;

    inline Object& dataRoot() noexcept;
    inline const Object& dataRoot() const noexcept;

    inline EnvironmentRootObject& environmentRoot() noexcept;
    inline const EnvironmentRootObject& environmentRoot() const noexcept;

  private:
    std::unique_ptr<Object> _dataRoot;
    std::unique_ptr<EnvironmentRootObject> _environmentRoot;
  };

  inline Object& EditorScene::dataRoot() noexcept
  {
    return *_dataRoot;
  }

  inline const Object& EditorScene::dataRoot() const noexcept
  {
    return *_dataRoot;
  }

  inline EnvironmentRootObject& EditorScene::environmentRoot() noexcept
  {
    return *_environmentRoot;
  }

  inline const EnvironmentRootObject&
                                  EditorScene::environmentRoot() const noexcept
  {
    return *_environmentRoot;
  }
}