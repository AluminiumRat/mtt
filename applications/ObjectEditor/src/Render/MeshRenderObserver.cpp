#include <stdexcept>

#include <mtt/application/Application.h>
#include <mtt/clPipeline/MeshTechniques/ModelCompositeColorTechnique.h>
#include <mtt/clPipeline/MeshTechniques/ShadowmapMeshTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/Mesh/DepthMeshTechnique.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/Box.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/Sphere.h>

#include <Objects/MeshObject.h>
#include <Render/MeshRenderObserver.h>

MeshRenderObserver::MeshRenderObserver( MeshObject& object,
                                        mtt::CommonEditData& commonData) :
  Object3DRenderObserver(object, commonData),
  _object(object),
  _mesh(mtt::Application::instance().displayDevice())
{
  _mesh.addModificator(_boneMatricesSetter);

  mtt::LogicalDevice& device = mtt::Application::instance().displayDevice();

  std::unique_ptr<mtt::clPipeline::ModelCompositeColorTechnique> colorTechnique;
  colorTechnique.reset(new mtt::clPipeline::ModelCompositeColorTechnique(
                                          true,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST));
  _mesh.setTechnique( mtt::clPipeline::colorFrameType,
                      std::move(colorTechnique));

  std::unique_ptr<mtt::UidMeshTechnique> uidTechnique(
                      new mtt::UidMeshTechnique(
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                                          true,
                                          true));
  _mesh.setTechnique( mtt::clPipeline::uidFrameType,
                      std::move(uidTechnique));

  std::unique_ptr<mtt::DepthMeshTechnique> depthTechnique;
  depthTechnique.reset(new mtt::DepthMeshTechnique(
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST));
  _mesh.setTechnique( mtt::clPipeline::depthmapFrameType,
                      std::move(depthTechnique));

  std::unique_ptr<mtt::clPipeline::ShadowmapMeshTechnique> shadowmapTechnique;
  shadowmapTechnique.reset(new mtt::clPipeline::ShadowmapMeshTechnique(
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST));
  _mesh.setTechnique( mtt::clPipeline::opaqueShadowmapFrameType,
                      std::move(shadowmapTechnique));

  _drawableNode.setDrawable(&_mesh, mtt::Sphere());
  registerCulledDrawable(_drawableNode);
  fullTransformJoint().addChild(_drawableNode);
  _drawableNode.addModificator(visibleFilter());
  _drawableNode.addModificator(uidSetter());
  _drawableNode.addModificator(selectionModificator());

  connect(&_object,
          &MeshObject::boneRefsChanged,
          this,
          &MeshRenderObserver::_updateMesh,
          Qt::DirectConnection);
  _updateMesh();

  connect(&_object,
          &MeshObject::geometryChanged,
          this,
          &MeshRenderObserver::_updateBones,
          Qt::DirectConnection);
  _updateBones();

  connect(&_object,
          &MeshObject::materialRefChanged,
          this,
          &MeshRenderObserver::_updateMaterialObserver,
          Qt::DirectConnection);
  _updateMaterialObserver();
}

void MeshRenderObserver::_updateMesh() noexcept
{
  try
  {
    mtt::Box boundingBox;
    for(const glm::vec3& position : _object.geometry().positions)
    {
      boundingBox.extend(position);
    }
    _drawableNode.setLocalBoundSphere(boundingBox.buildBoundingSphere());

    _mesh.setGeometry(_object.geometry());
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    mtt::Log() << "MeshRenderObserver::_updateMesh: unable to update the render mesh";
  }
  catch(...)
  {
    mtt::Log() << "MeshRenderObserver::_updateMesh: unable to update the render mesh";
  }
}

void MeshRenderObserver::_updateBones() noexcept
{
  _bonesObserver.reset();
  if(_object.boneRefs().boneRefsNumber() != 0)
  {
    try
    {
      _bonesObserver.emplace( _object,
                              _object.boneRefs(),
                              _mesh.extraData(),
                              _boneMatricesSetter);
    }
    catch (std::exception& error)
    {
      mtt::Log() << error.what();
      mtt::Abort("MeshRenderObserver::_updateBones: unable to update bone observer");
    }
    catch (...)
    {
      mtt::Abort("MeshRenderObserver::_updateBones: unable to update bone observer");
    }
  }
}

void MeshRenderObserver::_updateMaterialObserver() noexcept
{
  _materialObserver.reset();
  MaterialObject* material = _object.materialRef().get();
  if(material != nullptr)
  {
    try
    {
      _materialObserver.emplace(*material, _mesh.extraData());
    }
    catch(std::exception& error)
    {
      mtt::Log() << error.what();
      mtt::Abort("MeshRenderObserver::_updateMaterialObserver: unable to update material observer");
    }
    catch(...)
    {
      mtt::Abort("MeshRenderObserver::_updateMaterialObserver: unable to update material observer");
    }
  }
}
