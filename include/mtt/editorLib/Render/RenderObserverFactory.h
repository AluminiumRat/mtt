#pragma once

#include <memory>

#include <mtt/editorLib/Objects/CEVisitor.h>
#include <mtt/editorLib/Render/AbstractObjectRenderObserver.h>

namespace mtt
{
  class CommonEditData;
  class Object;
  class RenderScene;

  class RenderObserverFactory : public CEVisitor
  {
  public:
    RenderObserverFactory(CommonEditData& commonData);

    std::unique_ptr<AbstractObjectRenderObserver> buildObserver(
                                                      Object& object,
                                                      RenderScene& renderScene);

    virtual void visitAmbientLightObject(AmbientLightObject& object) override;
    virtual void visitBackgroundObject(BackgroundObject& object) override;
    virtual void visitDirectLightObject(DirectLightObject& object) override;
    virtual void visitEnvironmentModel(EnvironmentModel& object) override;
    virtual void visitFrameObject(FrameObject& object) override;
    virtual void visitSkeletonObject(SkeletonObject& object) override;

  protected:
    inline CommonEditData& commonData() const noexcept;
    inline RenderScene* renderScene() const noexcept;

    inline void setResult(
                std::unique_ptr<AbstractObjectRenderObserver> result) noexcept;

  private:
    CommonEditData& _commonData;
    RenderScene* _renderScene;
    std::unique_ptr<AbstractObjectRenderObserver> _result;
  };

  inline CommonEditData& RenderObserverFactory::commonData() const noexcept
  {
    return _commonData;
  }

  inline RenderScene* RenderObserverFactory::renderScene() const noexcept
  {
    return _renderScene;
  }

  inline void RenderObserverFactory::setResult(
                  std::unique_ptr<AbstractObjectRenderObserver> result) noexcept
  {
    _result.swap(result);
  }
}