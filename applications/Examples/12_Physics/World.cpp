#include <mtt/application/Application.h>
#include <mtt/clPipeline/MeshTechniques/ModelTechniquesFactory.h>

#include <Physics/PhysicsBoxObject.h>
#include <Physics/PhysicsCircleObject.h>
#include <Physics/PhysicsScene.h>
#include <World.h>

World::World( glm::vec2 size,
              RenderScene& renderScene,
              PhysicsScene& physicsScene) :
  _size(glm::max(size, glm::vec2(0.f))),
  _renderScene(renderScene),
  _physicsScene(physicsScene),
  _modelLibrary(std::make_unique<mtt::clPipeline::ModelTechniquesFactory>(
                                    true,   //color technique
                                    false,  //selection technique
                                    true,   //shadowmap technique
                                    false,  //depthmap technique
                                    false,  //id map technique
                                    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST),
                _textureLibrary)
{
}

void World::addSphere(glm::vec2 position, float radius, float invMass)
{
  std::unique_ptr<mtt::SlaveDrawModel> drawModel =
              _modelLibrary.load( ":/sphere.mmd",
                                  mtt::Application::instance().displayDevice());
  drawModel->setTransformMatrix(glm::scale(glm::vec3(radius)));

  std::unique_ptr<PhysicsCircleObject> physicsCircle(new PhysicsCircleObject());
  physicsCircle->setRadius(radius);
  physicsCircle->setInvMass(invMass);
  physicsCircle->setInvMomentOfInertia(5.f / 2.f * invMass / radius / radius);
  physicsCircle->setPosition(position);

  std::unique_ptr<Object> newObject(new Object(_physicsScene, _renderScene));
  newObject->setDrawable(std::move(drawModel));
  newObject->setPhysicsObject(std::move(physicsCircle));

  _objects.push_back(std::move(newObject));
}

void World::addBox( glm::vec2 position,
                    float rotation,
                    glm::vec2 size,
                    float invMass)
{
  std::unique_ptr<mtt::SlaveDrawModel> drawModel =
              _modelLibrary.load( ":/cube.mmd",
                                  mtt::Application::instance().displayDevice());
  glm::vec3 scale(size.x, (size.x + size.y) / 2.f, size.y);
  drawModel->setTransformMatrix(glm::scale(scale));

  std::unique_ptr<PhysicsBoxObject> physicsBox(new PhysicsBoxObject());
  physicsBox->setSize(size);
  physicsBox->setInvMass(invMass);
  physicsBox->setInvMomentOfInertia(12.f * invMass / glm::length2(size));
  physicsBox->setPosition(position);
  physicsBox->setRotation(rotation);

  std::unique_ptr<Object> newObject(new Object(_physicsScene, _renderScene));
  newObject->setDrawable(std::move(drawModel));
  newObject->setPhysicsObject(std::move(physicsBox));

  _objects.push_back(std::move(newObject));
}

void World::asyncWork(float dTime)
{
  _physicsScene.applyTime(dTime);
}

void World::synchronousWork()
{
  for (std::unique_ptr<Object>& object : _objects) object->synchronize();
  _removeExcessObjects();
}

void World::_removeExcessObjects() noexcept
{
  glm::vec2 halfSize = _size / 2.f;

  Objects::iterator iObject = _objects.begin();
  while (iObject != _objects.end())
  {
    glm::vec2 position = (*iObject)->physicsObject()->position();
    if (position.x > halfSize.x ||
        position.x < -halfSize.x ||
        position.y > halfSize.y ||
        position.y < -halfSize.x)
    {
      iObject = _objects.erase(iObject);
    }
    else iObject++;
  }
}
