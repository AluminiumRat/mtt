#pragma once

#include <glm/gtc/quaternion.hpp>

#include <Objects/Object3D.h>

class RotatableObject : public Object3D
{
  Q_OBJECT

  Q_PROPERTY( glm::quat rotation
              READ rotation
              WRITE setRotation
              RESET resetRotation
              NOTIFY rotationChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  RotatableObject(const QString& name,
                  bool canBeRenamed,
                  const mtt::UID& id = mtt::UID());
  RotatableObject(const RotatableObject&) = delete;
  RotatableObject& operator = (const RotatableObject&) = delete;
  virtual ~RotatableObject() noexcept = default;

  inline const glm::quat& rotation() const noexcept;
  virtual void setRotation(const glm::quat& newValue) noexcept;
  inline void resetRotation() noexcept;

signals:
  void rotationChanged(const glm::quat& newValue);

protected:
  virtual void updateLocalTransform() noexcept override;

private:
  glm::quat _rotation;
};

inline const glm::quat& RotatableObject::rotation() const noexcept
{
  return _rotation;
}

inline void RotatableObject::resetRotation() noexcept
{
  setRotation(glm::quat());
}
