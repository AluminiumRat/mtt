#pragma once

#include <glm/vec3.hpp>

#include <Objects/MovableObject.h>

class ScalableObject : public MovableObject
{
  Q_OBJECT

  Q_PROPERTY( glm::vec3 scale
              READ scale
              WRITE setScale
              RESET resetScale
              NOTIFY scaleChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  explicit ScalableObject(const mtt::UID& id = mtt::UID());
  ScalableObject(const ScalableObject&) = delete;
  ScalableObject& operator = (const ScalableObject&) = delete;
  virtual ~ScalableObject() noexcept = default;

  inline const glm::vec3& scale() const noexcept;
  virtual void setScale(const glm::vec3& newValue) noexcept;
  inline void resetScale() noexcept;

signals:
  void scaleChanged(const glm::vec3& newValue);

protected:
  virtual void updateLocalTransform() noexcept override;

private:
  glm::vec3 _scale;
};

inline const glm::vec3& ScalableObject::scale() const noexcept
{
  return _scale;
}

inline void ScalableObject::resetScale() noexcept
{
  setScale(glm::vec3(1));
}
