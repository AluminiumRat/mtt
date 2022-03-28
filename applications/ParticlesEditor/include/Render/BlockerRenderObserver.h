#pragma once

#pragma once

#include <mtt/editorLib/Render/HullDrawableNode.h>
#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/editorLib/Render/IconDrawableNode.h>

class BlockerObject;

class BlockerRenderObserver : public mtt::Object3DRenderObserver
{
  Q_OBJECT

public:
  BlockerRenderObserver(BlockerObject& object,
                        mtt::CommonEditData& commonData);
  BlockerRenderObserver(const BlockerRenderObserver&) = delete;
  BlockerRenderObserver& operator = (const BlockerRenderObserver&) = delete;
  virtual ~BlockerRenderObserver() noexcept = default;

private:
  void _updateHull() noexcept;

private:
  BlockerObject& _object;
  mtt::IconDrawableNode _iconNode;
  mtt::HullDrawableNode _hullNode;
};