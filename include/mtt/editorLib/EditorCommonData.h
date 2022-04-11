#pragma once

#include <QString>

#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/EditorScene.h>
#include <mtt/render/RenderScene.h>

namespace mtt
{
  class EditorCommonData : public mtt::CommonEditData
  {
    Q_OBJECT

  public:
    EditorCommonData(UndoStack& undoStack);
    EditorCommonData(const EditorCommonData&) = delete;
    EditorCommonData& operator = (const EditorCommonData&) = delete;
    virtual ~EditorCommonData() noexcept = default;

    inline EditorScene* scene() const noexcept;

    inline mtt::RenderScene& renderScene() noexcept;
    inline const mtt::RenderScene& renderScene() const noexcept;

    inline const QString& dataFilename() const noexcept;
    void setDataFilename(const QString& newValue) noexcept;

    inline const QString& environmentFilename() const noexcept;
    void setEnvironmentFilename(const QString& newValue) noexcept;

  signals:
    void sceneChanged(EditorScene* newScene);
    void dataFilenameChanged(const QString& newValue);
    void environmentFilenameChanged(const QString& newValue);

  protected:
    /// Returns old scene
    virtual std::unique_ptr<EditorScene> setScene(
                                        std::unique_ptr<EditorScene> newScene);

  private:
    mtt::RenderScene _renderScene;
    QString _dataFilename;
    QString _environmentFilename;
  };

  inline EditorScene* EditorCommonData::scene() const noexcept
  {
    return static_cast<EditorScene*>(CommonEditData::scene());
  }

  inline mtt::RenderScene& EditorCommonData::renderScene() noexcept
  {
    return _renderScene;
  }

  inline const mtt::RenderScene& EditorCommonData::renderScene() const noexcept
  {
    return _renderScene;
  }

  inline const QString& EditorCommonData::dataFilename() const noexcept
  {
    return _dataFilename;
  }

  inline const QString& EditorCommonData::environmentFilename() const noexcept
  {
    return _environmentFilename;
  }
}