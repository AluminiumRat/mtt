#include <algorithm>

#include <glm/glm.hpp>

#include <mtt/utilities/Abort.h>

#include <Physics/PhysicsObject.h>
#include <Physics/PhysicsScene.h>

void PhysicsScene::registerObject(PhysicsObject& newObject)
{
  Objects::iterator iObject = std::find(_objects.begin(),
                                        _objects.end(),
                                        &newObject);
  if(iObject != _objects.end()) mtt::Abort("PhysicsScene::registerObject: the object is already registered.");
  _objects.push_back(&newObject);
}

void PhysicsScene::unregisterObject(PhysicsObject& removedObject) noexcept
{
  Objects::iterator iObject = std::find(_objects.begin(),
                                        _objects.end(),
                                        &removedObject);
  if(iObject == _objects.end()) return;
  _objects.erase(iObject);
}

void PhysicsScene::applyTime(float dTime)
{
  dTime = std::max(dTime, 0.f);

  Contacts contactCollection;
  contactCollection.reserve(_objects.size() * 2);
  _processCollisions(contactCollection);

  _applyGravitation(dTime);
  _resolveContacts(contactCollection);

  for (PhysicsObject* object : _objects)
  {
    object->applyTime(dTime);
  }
}

void PhysicsScene::_applyGravitation(float dTime)
{
  for (PhysicsObject* object : _objects)
  {
    if (object->invMass() != 0.f)
    {
      Impulse impulse{};
      impulse._targetPoint = object->position();
      float force = 9.8f / object->invMass();
      impulse._impulseValue = force * dTime * glm::vec2(0.f, -1.f);
      object->applyImpulse(impulse);
    }
  }
}

void PhysicsScene::_resolveContacts(const Contacts& contacts)
{
  for (size_t i = 0; i < resolveIterationsNumber; i++)
  {
    for (const Contact& contact : contacts) contact.resolve();
  }
}

void PhysicsScene::_processCollisions(Contacts& contactsCollection)
{
  for(size_t object1Index = 0;
      object1Index + 1 < _objects.size();
      object1Index++)
  {
    for(size_t object2Index = object1Index + 1;
        object2Index < _objects.size();
        object2Index++)
    {
      PhysicsObject& object1 = *_objects[object1Index];
      PhysicsObject& object2 = *_objects[object2Index];
      
      //Objects with infinite masses should not interact
      if(object1.invMass() != 0.f || object2.invMass() != 0.f)
      {
        std::optional<Collision> collision = object1.intersect(object2);
        if (collision.has_value())
        {
          _pushObjects(object1, object2, *collision);
          contactsCollection.emplace_back(object1, object2, *collision);
        }
      }
    }
  }
}

void PhysicsScene::_pushObjects(PhysicsObject& object1,
                                PhysicsObject& object2,
                                const Collision& collision) noexcept
{
  float pushDistance = collision.penetration * .4f;
  pushDistance = std::max(0.f, pushDistance - .05f);
  float pushFactor = object1.invMass() + object2.invMass();
  if(pushFactor == 0.f) return;
  float object1Factor = object1.invMass() / pushFactor;
  object1.setPosition(object1.position() -
                              collision.normal * pushDistance * object1Factor);
  float object2Factor = object2.invMass() / pushFactor;
  object2.setPosition(object2.position() +
                              collision.normal * pushDistance * object2Factor);
}
