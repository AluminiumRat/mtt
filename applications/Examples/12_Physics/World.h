#pragma once

#include <memory>
#include <vector>

#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/application/ResourceManager/DrawModelLibrary.h>

#include <Object.h>

class PhysicsScene;
class RenderScene;

/// This is the central object that combines render and physics scene and
/// manages Object instances.
class World
{
public:
  /// size is the border size of world. Objects that are out of bounds will be
  /// removed.
  /// size must not be negative
  World(glm::vec2 size, RenderScene& renderScene, PhysicsScene& physicsScene);
  World(const World&) = delete;
  World& operator = (const World&) = delete;
  ~World() noexcept = default;

  inline glm::vec2 size() const noexcept;

  inline RenderScene& renderScene() const noexcept;
  inline PhysicsScene& physicsScene() const noexcept;

  void addSphere(glm::vec2 position, float radius, float invMass);
  void addBox(glm::vec2 position,
              float rotation,
              glm::vec2 size,
              float invMass);

  /// One step of simulation. Can be called async
  void asyncWork(float dTime);
  /// Synchronous part of simulation.
  void synchronousWork();

private:
  // Remove objects that are out of world bounds
  void _removeExcessObjects() noexcept;

private:
  glm::vec2 _size;

  RenderScene& _renderScene;
  PhysicsScene& _physicsScene;

  using Objects = std::vector<std::unique_ptr<Object>>;
  Objects _objects;

  // Storages for loadable models of sphere and box
  mtt::Texture2DLibrary _textureLibrary;
  mtt::DrawModelLibrary _modelLibrary;
};

inline glm::vec2 World::size() const noexcept
{
  return _size;
}

inline RenderScene& World::renderScene() const noexcept
{
  return _renderScene;
}

inline PhysicsScene& World::physicsScene() const noexcept
{
  return _physicsScene;
}
