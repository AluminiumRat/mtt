#pragma once

#include <memory>

#include <mtt/Render/Drawable/DrawableList.h>
#include <mtt/Render/SceneGraph/AbstractField.h>

namespace mtt
{
  class RenderScene
  {
  public:
    /// If field is nullptr UnsortedField will be used
    explicit RenderScene(std::unique_ptr<AbstractField> field = nullptr);
    RenderScene(const RenderScene&) = delete;
    RenderScene& operator = (const RenderScene&) = delete;
    virtual ~RenderScene() noexcept = default;

    inline AbstractField& culledData() noexcept;
    inline const AbstractField& culledData() const noexcept;

    inline DrawableList& unculledData() noexcept;
    inline const DrawableList& unculledData() const noexcept;

    inline DrawableList& hudData() noexcept;
    inline const DrawableList& hudData() const noexcept;

  private:
    std::unique_ptr<AbstractField> _culledData;
    DrawableList _unculledData;
    DrawableList _hudData;
  };

  inline AbstractField& RenderScene::culledData() noexcept
  {
    return *_culledData;
  }

  inline const AbstractField& RenderScene::culledData() const noexcept
  {
    return *_culledData;
  }

  inline DrawableList& RenderScene::unculledData() noexcept
  {
    return _unculledData;
  }

  inline const DrawableList& RenderScene::unculledData() const noexcept
  {
    return _unculledData;
  }

  inline DrawableList& RenderScene::hudData() noexcept
  {
    return _hudData;
  }

  inline const DrawableList& RenderScene::hudData() const noexcept
  {
    return _hudData;
  }
}