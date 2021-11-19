#pragma once

#include <memory>

#include <QtCore/QObject>

#include <mtt/Application/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/Render/Pipeline/Sampler.h>
#include <mtt/Utilities/SurfaceMaterialData.h>

#include <Objects/MaterialObject.h>

namespace mtt
{
  class MeshExtraData;
}

class MaterialObserver : public QObject
{
  Q_OBJECT

public:
  MaterialObserver( MaterialObject& material,
                    mtt::MeshExtraData& extraData);
  MaterialObserver(const MaterialObserver&) = delete;
  MaterialObserver& operator = (const MaterialObserver&) = delete;
  virtual ~MaterialObserver() noexcept = default;

private:
  using StopperUniquePtr = std::unique_ptr<mtt::AsyncTaskQueue::TaskStopper>;

private:
  void _updateMaterialData(const mtt::SurfaceMaterialData& newData) noexcept;
  void _updateTexture(const QString& filename,
                      const char* samplerName,
                      StopperUniquePtr& stopper) noexcept;
  void _updateAlbedoTexture(const QString& filename) noexcept;
  void _updateOpaqueTexture(const QString& filename) noexcept;
  void _updateUseAlpha(float value) noexcept;
  void _updateSpecularTexture(const QString& filename) noexcept;
  void _updateNormalTexture(const QString& filename) noexcept;
  void _updateEmissionTexture(const QString& filename) noexcept;
  void _updateReflectionTexture(const QString& filename) noexcept;

private:
  MaterialObject& _material;
  mtt::MeshExtraData& _extraData;

  mtt::SurfaceMaterialData _materialData;

  StopperUniquePtr _albedoTaskStopper;
  StopperUniquePtr _opaqueTaskStopper;
  StopperUniquePtr _specularTaskStopper;
  StopperUniquePtr _normalTaskStopper;
  StopperUniquePtr _emissionTaskStopper;
  StopperUniquePtr _reflectionTaskStopper;
};