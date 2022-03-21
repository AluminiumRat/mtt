#pragma once

#include <optional>

#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/editorLib/Render/IconDrawableNode.h>

class ObserverWithIcon : public mtt::Object3DRenderObserver
{
  Q_OBJECT

public:
  /// If iconFilename is empty or iconSize <= 0 then no icon node will be
  /// created
  ObserverWithIcon( mtt::Object3D& object,
                    mtt::CommonEditData& commonData,
                    const QString& iconFilename,
                    float iconSize);
  ObserverWithIcon(const ObserverWithIcon&) = delete;
  ObserverWithIcon& operator = (
                                const ObserverWithIcon&) = delete;
  virtual ~ObserverWithIcon() noexcept = default;

private:
  std::optional<mtt::IconDrawableNode> _iconNode;
};
