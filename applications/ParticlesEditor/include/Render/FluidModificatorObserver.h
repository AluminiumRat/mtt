#pragma once

#include <mtt/editorLib/Render/HullDrawableNode.h>
#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/editorLib/Render/IconDrawableNode.h>

class FluidModificator;

class FluidModificatorObserver : public mtt::Object3DRenderObserver
{
  Q_OBJECT

public:
  FluidModificatorObserver( FluidModificator& object,
                            const QString &iconFilename,
                            mtt::CommonEditData& commonData);
  FluidModificatorObserver(const FluidModificatorObserver&) = delete;
  FluidModificatorObserver& operator = (
                                      const FluidModificatorObserver&) = delete;
  virtual ~FluidModificatorObserver() noexcept = default;

private:
  void _updateHull() noexcept;

private:
  FluidModificator& _object;
  mtt::IconDrawableNode _iconNode;
  mtt::HullDrawableNode _hullNode;
};