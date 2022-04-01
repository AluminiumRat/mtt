#pragma once

#include <mtt/editorLib/Render/HullDrawableNode.h>
#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/editorLib/Render/IconDrawableNode.h>

class HeaterObject;

class HeaterRenderObserver : public mtt::Object3DRenderObserver
{
  Q_OBJECT

public:
  HeaterRenderObserver( HeaterObject& object,
                        mtt::CommonEditData& commonData);
  HeaterRenderObserver(const HeaterRenderObserver&) = delete;
  HeaterRenderObserver& operator = (const HeaterRenderObserver&) = delete;
  virtual ~HeaterRenderObserver() noexcept = default;

private:
  void _updateHull() noexcept;

private:
  HeaterObject& _object;
  mtt::IconDrawableNode _iconNode;
  mtt::HullDrawableNode _hullNode;
};