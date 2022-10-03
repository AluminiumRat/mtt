#pragma once

#include <vector>

#include <Physics/Contact.h>
#include <Physics/Impulse.h>

class Collision;
class PhysicsObject;

/// This is the main class of physics simulation. Contains references to
/// PhysicsObject instances and provides interaction between them
class PhysicsScene
{
public:
  static constexpr size_t resolveIterationsNumber = 10;

public:
  PhysicsScene() = default;
  PhysicsScene(const PhysicsScene&) = default;
  PhysicsScene& operator = (const PhysicsScene&) = default;
  ~PhysicsScene() noexcept = default;

  void registerObject(PhysicsObject& newObject);
  void unregisterObject(PhysicsObject& removedObject) noexcept;

  /// One step of physics simulation
  void applyTime(float dTime);

private:
  using Contacts = std::vector<Contact>;

  void _applyGravitation(float dTime);
  void _resolveContacts(const Contacts& contacts);
  void _processCollisions(Contacts& contactsCollection);
  void _pushObjects(PhysicsObject& object1,
                    PhysicsObject& object2,
                    const Collision& collision) noexcept;

private:
  using Objects = std::vector<PhysicsObject*>;
  Objects _objects;
};