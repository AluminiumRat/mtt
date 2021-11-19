#include <stdexcept>

#include <mtt/Render/Mesh/MeshExtraData.h>
#include <mtt/Utilities/Log.h>

#include <AsyncTasks/UploadTextureTask.h>
#include <Render/MaterialObserver.h>
#include <EditorApplication.h>

MaterialObserver::MaterialObserver( MaterialObject& material,
                                    mtt::MeshExtraData& extraData) :
  _material(material),
  _extraData(extraData)
{
  connect(&material,
          &MaterialObject::materialDataChanged,
          this,
          &MaterialObserver::_updateMaterialData,
          Qt::DirectConnection);
  _updateMaterialData(material.materialData());

  connect(&material,
          &MaterialObject::albedoTextureChanged,
          this,
          &MaterialObserver::_updateAlbedoTexture,
          Qt::DirectConnection);
  _updateAlbedoTexture(material.albedoTexture());

  connect(&material,
          &MaterialObject::useAlphaFromAlbedoTextureChanged,
          this,
          &MaterialObserver::_updateUseAlpha,
          Qt::DirectConnection);
  _updateUseAlpha(material.useAlphaFromAlbedoTexture());

  connect(&material,
          &MaterialObject::opaqueTextureChanged,
          this,
          &MaterialObserver::_updateOpaqueTexture,
          Qt::DirectConnection);
  _updateOpaqueTexture(material.opaqueTexture());

  connect(&material,
          &MaterialObject::specularTextureChanged,
          this,
          &MaterialObserver::_updateSpecularTexture,
          Qt::DirectConnection);
  _updateSpecularTexture(material.specularTexture());

  connect(&material,
          &MaterialObject::normalTextureChanged,
          this,
          &MaterialObserver::_updateNormalTexture,
          Qt::DirectConnection);
  _updateNormalTexture(material.normalTexture());

  connect(&material,
          &MaterialObject::emissionTextureChanged,
          this,
          &MaterialObserver::_updateEmissionTexture,
          Qt::DirectConnection);
  _updateEmissionTexture(material.emissionTexture());

  connect(&material,
          &MaterialObject::reflectionTextureChanged,
          this,
          &MaterialObserver::_updateReflectionTexture,
          Qt::DirectConnection);
  _updateReflectionTexture(material.reflectionTexture());
}

void MaterialObserver::_updateMaterialData(
                              const mtt::SurfaceMaterialData& newData) noexcept
{
  try
  {
    _extraData.setSurfaceMaterialData(newData);
  }
  catch(std::exception& error)
  {
    mtt::Log() << "MaterialObserver::_updateMaterialData: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "MaterialObserver::_updateMaterialData: unknown error.";
  }
}

void MaterialObserver::_updateTexture(const QString& filename,
                                      const char* samplerName,
                                      StopperUniquePtr& stopper) noexcept
{
  try
  {
    if(filename.isEmpty()) _extraData.removeSampler(samplerName);
    else
    {
      std::unique_ptr<UploadTextureTask> task;
      task.reset(new UploadTextureTask(filename, _extraData, samplerName));
      mtt::AsyncTaskQueue& queue =
                                  EditorApplication::instance().asyncTaskQueue;
      stopper = queue.addTaskWithStopper(std::move(task));
    }
  }
  catch (std::exception& error)
  {
    mtt::Log() << "MaterialObserver::_updateTexture: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "MaterialObserver::_updateTexture: unknown error.";
  }
}

void MaterialObserver::_updateAlbedoTexture(const QString& filename) noexcept
{
  _updateTexture( filename,
                  mtt::MeshExtraData::albedoSamplerName,
                  _albedoTaskStopper);
}

void MaterialObserver::_updateUseAlpha(float value) noexcept
{
  try
  {
    _extraData.setAlphaInAlbedoSamplerIsOpacity(value);
  }
  catch (std::exception& error)
  {
    mtt::Log() << "MaterialObserver::_updateUseAlpha: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "MaterialObserver::_updateUseAlpha: unknown error.";
  }
}

void MaterialObserver::_updateOpaqueTexture(const QString& filename) noexcept
{
  _updateTexture( filename,
                  mtt::MeshExtraData::opaqueSamplerName,
                  _opaqueTaskStopper);
}

void MaterialObserver::_updateSpecularTexture(const QString& filename) noexcept
{
  _updateTexture( filename,
                  mtt::MeshExtraData::specularSamplerName,
                  _specularTaskStopper);
}

void MaterialObserver::_updateNormalTexture(const QString& filename) noexcept
{
  _updateTexture( filename,
                  mtt::MeshExtraData::normalSamplerName,
                  _normalTaskStopper);
}

void MaterialObserver::_updateEmissionTexture(const QString& filename) noexcept
{
  _updateTexture( filename,
                  mtt::MeshExtraData::emissionSamplerName,
                  _emissionTaskStopper);
}

void MaterialObserver::_updateReflectionTexture(
                                              const QString& filename) noexcept
{
  _updateTexture( filename,
                  mtt::MeshExtraData::reflectionSamplerName,
                  _reflectionTaskStopper);
}
