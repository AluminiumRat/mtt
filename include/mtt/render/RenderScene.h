#pragma once

#include <memory>

#include <mtt/render/Drawable/DrawableList.h>
#include <mtt/render/SceneGraph/AbstractField.h>
#include <mtt/render/CompositeRenderObject.h>

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

    inline void addCulledDrawable(DrawableNode& node);
    inline void removeCulledDrawable(DrawableNode& node) noexcept;

    inline void addAreaModificator(AreaModificator& modificator);
    inline void removeAreaModificator(AreaModificator& modificator) noexcept;

    inline void addUnculledDrawable(Drawable& newChild);
    inline void removeUnculledDrawable(Drawable& child) noexcept;

    inline void addHUDDrawable(Drawable& newChild);
    inline void removeHUDDrawable(Drawable& child) noexcept;

    inline void addCompositeObject(CompositeRenderObject& composite);
    inline void removeCompositeObject(
                                    CompositeRenderObject& composite) noexcept;

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

  inline void RenderScene::addCulledDrawable(DrawableNode& node)
  {
    _culledData->addDrawable(node);
  }

  inline void RenderScene::removeCulledDrawable(DrawableNode& node) noexcept
  {
    _culledData->removeDrawable(node);
  }

  inline void RenderScene::addAreaModificator(AreaModificator& modificator)
  {
    _culledData->addModificator(modificator);
  }

  inline void RenderScene::removeAreaModificator(
                                          AreaModificator& modificator) noexcept
  {
    _culledData->removeModificator(modificator);
  }

  inline void RenderScene::addUnculledDrawable(Drawable& newChild)
  {
    _unculledData.addChild(newChild);
  }

  inline void RenderScene::removeUnculledDrawable(Drawable& child) noexcept
  {
    _unculledData.removeChild(child);
  }

  inline void RenderScene::addHUDDrawable(Drawable& newChild)
  {
    _hudData.addChild(newChild);
  }

  inline void RenderScene::removeHUDDrawable(Drawable& child) noexcept
  {
    _hudData.removeChild(child);
  }

  inline void RenderScene::addCompositeObject(CompositeRenderObject& composite)
  {
    try
    {
      for(size_t drawableIndex = 0;
          drawableIndex < composite.culledDrawablesNumber();
          drawableIndex++)
      {
        addCulledDrawable(composite.culledDrawable(drawableIndex));
      }

      for(size_t modificatorIndex = 0;
          modificatorIndex < composite.areaModificatorsNumber();
          modificatorIndex++)
      {
        addAreaModificator(composite.areaModificator(modificatorIndex));
      }

      for(size_t drawableIndex = 0;
          drawableIndex < composite.unculledDrawablesNumber();
          drawableIndex++)
      {
        addUnculledDrawable(composite.unculledDrawable(drawableIndex));
      }

      for(size_t drawableIndex = 0;
          drawableIndex < composite.hudDrawablesNumber();
          drawableIndex++)
      {
        addHUDDrawable(composite.hudDrawable(drawableIndex));
      }
    }
    catch (...)
    {
      removeCompositeObject(composite);
      throw;
    }
  }

  inline void RenderScene::removeCompositeObject(
                                    CompositeRenderObject& composite) noexcept
  {
    for(size_t drawableIndex = 0;
        drawableIndex < composite.culledDrawablesNumber();
        drawableIndex++)
    {
      removeCulledDrawable(composite.culledDrawable(drawableIndex));
    }

    for(size_t modificatorIndex = 0;
        modificatorIndex < composite.areaModificatorsNumber();
        modificatorIndex++)
    {
      removeAreaModificator(composite.areaModificator(modificatorIndex));
    }

    for(size_t drawableIndex = 0;
        drawableIndex < composite.unculledDrawablesNumber();
        drawableIndex++)
    {
      removeUnculledDrawable(composite.unculledDrawable(drawableIndex));
    }

    for(size_t drawableIndex = 0;
        drawableIndex < composite.hudDrawablesNumber();
        drawableIndex++)
    {
      removeHUDDrawable(composite.hudDrawable(drawableIndex));
    }
  }

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