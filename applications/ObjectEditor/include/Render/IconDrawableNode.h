#pragma once

#include <QtCore/QString>

#include <mtt/Render/Drawable/AutoscaleDrawableModificator.h>
#include <mtt/Render/Drawable/RotateToCameraModificator.h>
#include <mtt/Render/Mesh/Mesh.h>
#include <mtt/Render/SceneGraph/SimpleDrawableNode.h>

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