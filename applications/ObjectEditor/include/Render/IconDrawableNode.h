#pragma once

#include <QtCore/QString>

#include <mtt/render/Drawable/AutoscaleDrawableModificator.h>
#include <mtt/render/Drawable/RotateToCameraModificator.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

class IconDrawableNode : public mtt::SimpleDrawableNode
{
public:
  /// iconSize in pixels
  IconDrawableNode(const QString& imageFilename, float iconSize) noexcept;
  IconDrawableNode(const IconDrawableNode&) = delete;
  IconDrawableNode& operator = (const IconDrawableNode&) = delete;
  virtual ~IconDrawableNode() noexcept = default;

private:
  mtt::Mesh _iconMesh;
  mtt::AutoscaleDrawableModificator _autoscale;
  mtt::RotateToCameraModificator _autorotate;
};