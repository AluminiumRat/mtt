#pragma once

#include <glm/vec3.hpp>

#include <mtt/editorLib/Objects/MovableObject.h>

#include <Objects/PEVisitorExtension.h>

class ParticleField : public mtt::MovableObject
{
  Q_OBJECT
  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitParticleField,
                          visitConstParticleField,
                          mtt::MovableObject)

  Q_PROPERTY( glm::vec3 size
              READ size
              WRITE setSize
              RESET resetSize
              NOTIFY sizeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  ParticleField(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  ParticleField(const ParticleField&) = delete;
  ParticleField& operator = (const ParticleField&) = delete;
  virtual ~ParticleField() noexcept = default;

  inline const glm::vec3& size() const noexcept;
  void setSize(const glm::vec3& newValue);
  inline void resetSize();

signals:
  void sizeChanged(const glm::vec3& newValue);

private:
  glm::vec3 _size;
};

inline const glm::vec3& ParticleField::size() const noexcept
{
  return _size;
}

inline void ParticleField::resetSize()
{
  setSize(glm::vec3(10.f, 10.f, 10.f));
}
