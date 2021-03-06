#include <algorithm>
#include <random>
#include <stdexcept>

#include <mtt/application/Application.h>
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
  _particlesDrawable(mtt::Application::instance().displayDevice()),
  Object3DRenderObserver(object, commonData),
  _field(object)
{
  std::default_random_engine randomEngine;
  std::uniform_int_distribution<int> distribution(0, 255);
  for (uint8_t& value : _randomTable)
  {
    value = uint8_t(distribution(randomEngine));
  }

  registerCulledDrawable(_hullNode);
  fullTransformJoint().addChild(_hullNode);
  _hullNode.addModificator(visibleFilter());
  _hullNode.addModificator(uidSetter());
  _hullNode.addModificator(selectionModificator());

  _particlesNode.setDrawable(&_particlesDrawable, mtt::Sphere());
  registerCulledDrawable(_particlesNode);
  fullTransformJoint().addChild(_particlesNode);
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
          &ParticleField::textureDescriptionsChanged,
          this,
          &FieldRenderObserver::_updateTextures,
          Qt::DirectConnection);
  _updateTextures();

  connect(&_field,
          &ParticleField::lodMppxChanged,
          this,
          &FieldRenderObserver::_updateLodMppx,
          Qt::DirectConnection);
  _updateLodMppx();

  connect(&_field,
          &ParticleField::lodSmoothingChanged,
          this,
          &FieldRenderObserver::_updateLodSmoothing,
          Qt::DirectConnection);
  _updateLodSmoothing();

  connect(&_field,
          &ParticleField::lightingTypeChanged,
          this,
          &FieldRenderObserver::_updateLightingType,
          Qt::DirectConnection);
  _updateLightingType();
}

void FieldRenderObserver::_updateSize() noexcept
{
  glm::vec3 size = _field.size();

  try
  {
    _hullNode.setBoxGeometry(size);
  }
  catch (std::exception& error)
  {
    mtt::Log() << "FieldRenderObserver::_updateSize: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "FieldRenderObserver::_updateSize: unknown error";
  }

  mtt::Sphere boundSphere(glm::vec3(0.f), glm::length(size) / 2.f);
  _particlesNode.setLocalBoundSphere(boundSphere);
}

void FieldRenderObserver::_updateParticles() noexcept
{
  size_t pointsNumber = _field.workIndices().size();

  std::vector<glm::vec3> positions;
  positions.reserve(pointsNumber);

  std::vector<glm::vec2> sizeRotation;
  sizeRotation.reserve(pointsNumber);

  std::vector<glm::vec4> albedo;
  albedo.reserve(pointsNumber);

  std::vector<glm::vec4> emission;
  emission.reserve(pointsNumber);

  std::vector<uint32_t> textureIndices;
  textureIndices.reserve(pointsNumber);

  std::vector<uint32_t> tileIndices;
  tileIndices.reserve(pointsNumber);

  std::vector<uint8_t> tagValues;
  tagValues.reserve(pointsNumber);

  for (ParticleField::ParticleIndex index : _field.workIndices())
  {
    ParticleField::ParticleData particle = _field.particlesData()[index];
    positions.push_back(particle.position);
    sizeRotation.push_back(glm::vec2(particle.size, particle.rotation));
    float opacity = particle.albedo.a;
    glm::vec3 premultipledAlbedo = particle.albedo;
    premultipledAlbedo *= opacity;
    albedo.push_back(glm::vec4(premultipledAlbedo, opacity) *
                                                    particle.visibilityFactor);
    emission.push_back(
                glm::vec4(particle.emission * particle.visibilityFactor, 1.f));
    textureIndices.push_back(particle.textureIndex);
    tileIndices.push_back(particle.tileIndex);
    tagValues.push_back(_randomTable[index % 256]);
  }

  _particlesDrawable.setData( positions,
                              sizeRotation,
                              albedo,
                              emission,
                              textureIndices,
                              tileIndices,
                              tagValues);
}

void FieldRenderObserver::_updateTextures() noexcept
{
  try
  {
    if (_field.textureDescriptions().empty())
    {
      _particlesDrawable.setParticleTextures({});
      return;
    }

    std::unique_ptr<UploadParticleTexturesTask> task;
    task.reset(new UploadParticleTexturesTask(_field.textureDescriptions(),
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

void FieldRenderObserver::_updateLodMppx() noexcept
{
  _particlesDrawable.setFalloffBaseMppx(_field.lodMppx());
}

void FieldRenderObserver::_updateLodSmoothing() noexcept
{
  _particlesDrawable.setFalloffSmoothing(_field.lodSmoothing());
}

void FieldRenderObserver::_updateLightingType() noexcept
{
  _particlesDrawable.setLightingType(
      static_cast<mtt::ParticlesDrawable::LightingType>(_field.lightingType()));
}
