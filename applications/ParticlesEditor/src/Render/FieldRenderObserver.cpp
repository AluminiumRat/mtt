#include <stdexcept>

#include <mtt/clPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/utilities/Log.h>

#include <Objects/ParticleField.h>
#include <Render/FieldRenderObserver.h>

FieldRenderObserver::FieldRenderObserver( ParticleField& object,
                                          mtt::CommonEditData& commonData) :
  Object3DRenderObserver(object, commonData),
  _field(object),
  _boxMesh(mtt::EditorApplication::instance().displayDevice())
{
  _boxMesh.setTechnique(
        mtt::clPipeline::colorFrameType,
        std::make_unique<mtt::clPipeline::InstrumentalCompositeTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));
  _boxMesh.setTechnique(mtt::clPipeline::uidFrameType,
                        std::make_unique<mtt::UidMeshTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));

  mtt::SurfaceMaterialData materialData;
  materialData.albedo = glm::vec3(.7f, .7f, .4f);
  materialData.roughness = 1;
  materialData.specularStrength = 1;
  _boxMesh.extraData().setSurfaceMaterialData(materialData);

  _boxNode.setDrawable(&_boxMesh, mtt::Sphere());
  registerCulledDrawable(_boxNode);
  positionRotateJoint().addChild(_boxNode);
  _boxNode.addModificator(visibleFilter());
  _boxNode.addModificator(uidSetter());
  _boxNode.addModificator(selectionModificator());

  connect(&_field,
          &ParticleField::sizeChanged,
          this,
          &FieldRenderObserver::_updateBox,
          Qt::DirectConnection);
  _updateBox();
}

void FieldRenderObserver::_updateBox() noexcept
{
  glm::vec3 size = _field.size();
  glm::vec3 halfSize = size / 2.f;

  try
  {
    std::vector<glm::vec3> newVertices;
    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y,  halfSize.z));
    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y,  halfSize.z));

    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y,  halfSize.z));
    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y,  halfSize.z));

    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y,  halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x,  halfSize.y,  halfSize.z));

    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y,  halfSize.z));
    newVertices.push_back(glm::vec3( halfSize.x,  halfSize.y,  halfSize.z));

    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3( halfSize.x,  halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x,  halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3(-halfSize.x, halfSize.y, halfSize.z));
    newVertices.push_back(glm::vec3(halfSize.x, halfSize.y, halfSize.z));

    newVertices.push_back(glm::vec3(halfSize.x, halfSize.y, halfSize.z));
    newVertices.push_back(glm::vec3(halfSize.x, halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3(halfSize.x, halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x, halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3(-halfSize.x, halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x, halfSize.y, halfSize.z));

    mtt::LogicalDevice& device =
                            mtt::EditorApplication::instance().displayDevice();
    std::shared_ptr<mtt::Buffer> positionsBuffer(
                          new mtt::Buffer(device, mtt::Buffer::VERTEX_BUFFER));
    positionsBuffer->setData( newVertices.data(),
                              newVertices.size() * sizeof(glm::vec3));
    _boxMesh.setPositionBuffer(positionsBuffer);

    _boxMesh.setVerticesNumber(uint32_t(newVertices.size()));
  }
  catch (std::exception& error)
  {
    mtt::Log() << "FieldRenderObserver::_updateBox: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "FieldRenderObserver::_updateBox: unknown error";
  }

  float radius = glm::length(size) / 2;
  _boxNode.setLocalBoundSphere(mtt::Sphere(glm::vec3(0.f), radius));
}
