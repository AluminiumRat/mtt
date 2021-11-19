#pragma once

#include <glm/vec3.hpp>

#include <Objects/RotatableObject.h>

class MovableObject : public RotatableObject
{
  Q_OBJECT

  Q_PROPERTY( glm::vec3 position
              READ position
              WRITE setPosition
              RESET resetPosition
              NOTIFY positionChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  explicit MovableObject(const mtt::UID& id = mtt::UID());
  MovableObject(const MovableObject&) = delete;
  MovableObject& operator = (const MovableObject&) = delete;
  virtual ~MovableObject() noexcept = default;

  inline const glm::vec3& position() const noexcept;
  virtual void setPosition(const glm::vec3& newValue) noexcept;
  inline void resetPosition() noexcept;

signals:
  void positionChanged(const glm::vec3& newValue);

protected:
  virtual void updateLocalTransform() noexcept override;

private:
  glm::vec3 _position;
};

inline const glm::vec3& MovableObject::position() const noexcept
{
  return _position;
}

inline void MovableObject::resetPosition() noexcept
{
  setPosition(glm::vec3(0));
}
