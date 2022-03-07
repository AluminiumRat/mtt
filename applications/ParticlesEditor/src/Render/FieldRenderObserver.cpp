#include <algorithm>
#include <stdexcept>

#include <mtt/clPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/utilities/Abort.h>

#include <AsyncTasks/UploadParticleTexturesTask.h>
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

  _particlesNode.setDrawable(&_particlesDrawable, mtt::Sphere());
  registerCulledDrawable(_particlesNode);
  positionRotateJoint().addChild(_particlesNode);
  _particlesNode.addModificator(visibleFilter());
  _particlesNode.addModificator(uidSetter());
  _particlesNode.addModificator(selectionModificator());

  connect(&_field,
          &ParticleField::sizeChanged,
          this,
          &FieldRenderObserver::_updateSize,
          Qt::DirectConnection);
  _updateSize();

  connect(&_field,
          &ParticleField::cleared,
          this,
          &FieldRenderObserver::_updateParticles,
          Qt::DirectConnection);
  connect(&_field,
          &ParticleField::simulationStepFinished,
          this,
          &FieldRenderObserver::_updateParticles,
          Qt::DirectConnection);
  _updateParticles();

  connect(&_field,
          &ParticleField::textureFilesChanged,
          this,
          &FieldRenderObserver::_updateTextures,
          Qt::DirectConnection);
  _updateTextures();
}

void FieldRenderObserver::_updateSize() noexcept
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

  mtt::Sphere boundSphere(glm::vec3(0.f), glm::length(size) / 2.f);
  _boxNode.setLocalBoundSphere(boundSphere);
  _particlesNode.setLocalBoundSphere(boundSphere);
}

void FieldRenderObserver::_updateParticles() noexcept
{
  std::vector<glm::vec3> positions;
  std::vector<glm::vec2> sizeRotation;
  std::vector<glm::vec4> color;
  std::vector<uint32_t> textureIndices;

  for (ParticleField::ParticleIndex index : _field.workIndices())
  {
    ParticleField::ParticleData particle = _field.particlesData()[index];
    positions.push_back(particle.position);
    sizeRotation.push_back(glm::vec2(particle.size, particle.rotation));
    color.push_back(glm::vec4(particle.color * particle.brightness,
                              particle.opacity));
    textureIndices.push_back(particle.textureIndex);
  }

  _particlesDrawable.setData( positions,
                              sizeRotation,
                              color,
                              textureIndices);
}

void FieldRenderObserver::_updateTextures() noexcept
{
  try
  {
    if (_field.textureFiles().empty())
    {
      _particlesDrawable.setParticleTextures({});
      return;
    }

    std::unique_ptr<UploadParticleTexturesTask> task;
    task.reset(new UploadParticleTexturesTask(_field.textureFiles(),
                                              _particlesDrawable));
    mtt::AsyncTaskQueue& queue =
                              mtt::EditorApplication::instance().asyncTaskQueue;
    _uploadTextureStopper = queue.addTaskWithStopper(std::move(task));
  }
  catch (std::exception& error)
  {
    mtt::Log() << "FieldRenderObserver::_updateTextures: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "FieldRenderObserver::_updateTextures: unknown error.";
  }
}
