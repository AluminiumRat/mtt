#pragma once

#include <QtCore/QString>

#include <mtt/render/Drawable/AutoscaleDrawableModificator.h>
#include <mtt/render/Drawable/RotateToCameraModificator.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

namespace mtt
{
  class IconDrawableNode : public SimpleDrawableNode
  {
  public:
    /// iconSize in pixels
    IconDrawableNode(const QString& imageFilename, float iconSize);
    IconDrawableNode(const IconDrawableNode&) = delete;
    IconDrawableNode& operator = (const IconDrawableNode&) = delete;
    virtual ~IconDrawableNode() noexcept = default;

  private:
    Mesh _iconMesh;
    AutoscaleDrawableModificator _autoscale;
    RotateToCameraModificator _autorotate;
  };
}