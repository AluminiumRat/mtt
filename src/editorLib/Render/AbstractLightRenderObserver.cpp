#include <stdexcept>

#include <mtt/clPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/editorLib/Render/AbstractLightRenderObserver.h>
#include <mtt/editorLib/Objects/LightObject.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/render/SceneGraph/CompositeObjectNode.h>
#include <mtt/utilities/Box.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

AbstractLightRenderObserver::AbstractLightRenderObserver(
                                                    LightObject& object,
                                                    CommonEditData& commonData,
                                                    const QString& iconFilename,
                                                    float iconSize) :
  Object3DRenderObserver(object, commonData),
  _object(object),
  _lightRenderer(nullptr),
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
        clPipeline::colorFrameType,
        std::make_unique<clPipeline::InstrumentalCompositeTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));
  _hullMesh.setTechnique( clPipeline::uidFrameType,
                          std::make_unique<UidMeshTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));

  SurfaceMaterialData materialData;
  materialData.albedo = glm::vec3(.7f, .7f, .4f);
  materialData.roughness = 1;
  materialData.specularStrength = 1;
  _hullMesh.extraData().setSurfaceMaterialData(materialData);

  _hullNode.setDrawable(&_hullMesh, Sphere());
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

void AbstractLightRenderObserver::setLightObject(CompositeObjectNode& light)
{
  _clearLightRenderer();

  try
  {
    _lightRenderer = &light;
    positionRotateJoint().addChild(*_lightRenderer);
    if(_object.enabled()) registerCompositeObject(*_lightRenderer);
  }
  catch (...)
  {
    _clearLightRenderer();
    throw;
  }
}

void AbstractLightRenderObserver::_clearLightRenderer() noexcept
{
  if(_lightRenderer != nullptr && _object.enabled())
  {
    unregisterCompositeObject(*_lightRenderer);
  }
}

void AbstractLightRenderObserver::_updateEnabled() noexcept
{
  if (_lightRenderer != nullptr)
  {
    try
    {
      if(_object.enabled()) registerCompositeObject(*_lightRenderer);
      else unregisterCompositeObject(*_lightRenderer);
    }
    catch (std::exception& error)
    {
      Log() << "AbstractLightRenderObserver::_updateEnabled: " << error.what();
    }
    catch (...)
    {
      Log() << "AbstractLightRenderObserver::_updateEnabled: unable to register area modificator";
    }
  }
}

void AbstractLightRenderObserver::setHullGeometry(
                                      const std::vector<glm::vec3> newGeometry)
{
  try
  {
    Box boundingBox;
    for(const glm::vec3& point : newGeometry) boundingBox.extend(point);
    _hullNode.setLocalBoundSphere(boundingBox.buildBoundingSphere());

    LogicalDevice& device = EditorApplication::instance().displayDevice();
    std::shared_ptr<Buffer> positionsBuffer(new Buffer(device,
                                            Buffer::VERTEX_BUFFER));
    positionsBuffer->setData( newGeometry.data(),
                              newGeometry.size() * sizeof(glm::vec3));
    _hullMesh.setPositionBuffer(positionsBuffer);

    _hullMesh.setVerticesNumber(uint32_t(newGeometry.size()));
  }
  catch (...)
  {
    _hullNode.setLocalBoundSphere(Sphere());
    _hullMesh.setVerticesNumber(0);
  }
}
