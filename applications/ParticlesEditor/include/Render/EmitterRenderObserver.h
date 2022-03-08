#pragma once

#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/editorLib/Render/IconDrawableNode.h>

class EmitterObject;

class EmitterRenderObserver : public mtt::Object3DRenderObserver
{
  Q_OBJECT

public:
  EmitterRenderObserver(EmitterObject& object,
                        mtt::CommonEditData& commonData);
  EmitterRenderObserver(const EmitterRenderObserver&) = delete;
  EmitterRenderObserver& operator = (
    const EmitterRenderObserver&) = delete;
  virtual ~EmitterRenderObserver() noexcept = default;

private:
  EmitterObject& _emitter;
  mtt::IconDrawableNode _iconNode;
};