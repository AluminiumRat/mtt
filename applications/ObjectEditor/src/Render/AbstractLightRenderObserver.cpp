#include <stdexcept>

#include <mtt/clPipeline/Lighting/AbstractLight.h>
#include <mtt/clPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/utilities/Box.h>
#include <mtt/utilities/Log.h>

#include <Objects/LightObject.h>
#include <Render/AbstractLightRenderObserver.h>
#include <EditorApplication.h>

AbstractLightRenderObserver::AbstractLightRenderObserver(
                                                LightObject& object,
                                                EditorCommonData& commonData,
                                                const QString& iconFilename,
                                                float iconSize) :
  Object3DRenderObserver(object, commonData),
  _object(object),
  _infinityArea(false),
  _defferedLightApplicator(nullptr),
  _forwardLightApplicator(nullptr),
  _hullMesh(EditorApplication::instance().displayDevice())
{
  if (!iconFilename.isEmpty() && iconSize > 0)
  {
    _iconNode.emplace(iconFilename, iconSize);
    registerUnculledDrawable(*_iconNode);
    positionJoint().addChild(*_iconNode);
    _iconNode->addModificator(visibleFilter());
    _iconNode->addModificator(uidSetter());
    _iconNode->addModificator(selectionModificator());
  }

  _hullMesh.setTechnique(
        mtt::clPipeline::colorFrameType,
        std::make_unique<mtt::clPipeline::InstrumentalCompositeTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));
  _hullMesh.setTechnique( mtt::clPipeline::uidFrameType,
                          std::make_unique<mtt::UidMeshTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));

  mtt::SurfaceMaterialData materialData;
  materialData.albedo = glm::vec3(.7f, .7f, .4f);
  materialData.roughness = 1;
  materialData.specularStrength = 1;
  _hullMesh.extraData().setSurfaceMaterialData(materialData);

  _hullNode.setDrawable(&_hullMesh, mtt::Sphere());
  registerCulledDrawable(_hullNode);
  positionRotateJoint().addChild(_hullNode);
  _hullNode.addModificator(visibleFilter());
  _hullNode.addModificator(uidSetter());
  _hullNode.addModificator(selectionModificator());

  connect(&_object,
          &LightObject::enabledChanged,
          this,
          &AbstractLightRenderObserver::_updateEnabled,
          Qt::DirectConnection);
  _updateEnabled();
}

void AbstractLightRenderObserver::setLightObject(
                                        mtt::clPipeline::AbstractLight& light)
{
  _clearDrawables();

  _defferedLightApplicator = light.defferedLightApplicator();
  _forwardLightApplicator = light.forwardLightModificator();

  try
  {
    if (_defferedLightApplicator != nullptr)
    {
      _defferedLightApplicator->addModificator(_enableFilter);
      positionRotateJoint().addChild(*_defferedLightApplicator);
      if(_infinityArea) registerUnculledDrawable(*_defferedLightApplicator);
      else registerCulledDrawable(*_defferedLightApplicator);
    }
    if (_forwardLightApplicator != nullptr)
    {
      positionRotateJoint().addChild(*_forwardLightApplicator);
      if(_object.enabled()) registerAreaModificator(*_forwardLightApplicator);
    }
  }
  catch (...)
  {
    _clearDrawables();
    throw;
  }
}

void AbstractLightRenderObserver::setInfinityArea(bool newValue)
{
  if(_infinityArea == newValue) return;

  _infinityArea = newValue;

  if(_defferedLightApplicator == nullptr) return;

  try
  {
    if (_infinityArea)
    {
      unregisterCulledDrawable(*_defferedLightApplicator);
      registerUnculledDrawable(*_defferedLightApplicator);
    }
    else
    {
      unregisterUnculledDrawable(*_defferedLightApplicator);
      registerCulledDrawable(*_defferedLightApplicator);
    }
  }
  catch (std::exception& error)
  {
    mtt::Log() << "AbstractLightRenderObserver::setInfinityArea: unable to registed deffered light applicator: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "AbstractLightRenderObserver::setInfinityArea: unable to registed deffered light applicator";
  }
}

void AbstractLightRenderObserver::_clearDrawables() noexcept
{
  if (_defferedLightApplicator != nullptr)
  {
    _defferedLightApplicator->removeModificator(_enableFilter);
    positionRotateJoint().removeChild(*_defferedLightApplicator);
    if (_infinityArea) unregisterUnculledDrawable(*_defferedLightApplicator);
    else unregisterCulledDrawable(*_defferedLightApplicator);
    _defferedLightApplicator = nullptr;
  }
  if (_forwardLightApplicator != nullptr)
  {
    positionRotateJoint().removeChild(*_forwardLightApplicator);
    if (_object.enabled()) unregisterAreaModificator(*_forwardLightApplicator);
    _forwardLightApplicator = nullptr;
  }
}

void AbstractLightRenderObserver::_updateEnabled() noexcept
{
  _enableFilter.setVisible(_object.enabled());
  if (_forwardLightApplicator != nullptr)
  {
    try
    {
      if(_object.enabled()) registerAreaModificator(*_forwardLightApplicator);
      else unregisterAreaModificator(*_forwardLightApplicator);
    }
    catch (std::exception& error)
    {
      mtt::Log() << "AbstractLightRenderObserver::_updateEnabled: " << error.what();
    }
    catch (...)
    {
      mtt::Log() << "AbstractLightRenderObserver::_updateEnabled: unable to register area modificator";
    }
  }
}

void AbstractLightRenderObserver::setHullGeometry(
                                      const std::vector<glm::vec3> newGeometry)
{
  try
  {
    mtt::Box boundingBox;
    for(const glm::vec3& point : newGeometry) boundingBox.extend(point);
    _hullNode.setLocalBoundSphere(boundingBox.buildBoundingSphere());

    mtt::LogicalDevice& device = EditorApplication::instance().displayDevice();
    std::shared_ptr<mtt::Buffer> positionsBuffer(
                                    new mtt::Buffer(device,
                                                    mtt::Buffer::VERTEX_BUFFER));
    positionsBuffer->setData( newGeometry.data(),
                              newGeometry.size() * sizeof(glm::vec3));
    _hullMesh.setPositionBuffer(positionsBuffer);

    _hullMesh.setVerticesNumber(uint32_t(newGeometry.size()));
  }
  catch (...)
  {
    _hullNode.setLocalBoundSphere(mtt::Sphere());
    _hullMesh.setVerticesNumber(0);
  }
}
