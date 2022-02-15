#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <mtt/editorLib/Objects/CEVisitorExtension.h>
#include <mtt/editorLib/Objects/DisplayedObject.h>

namespace mtt
{
  class Object3D : public DisplayedObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension)

  public:
    Object3D( const QString& name,
              bool canBeRenamed,
              const UID& id = UID());
    Object3D(const Object3D&) = delete;
    Object3D& operator = (const Object3D&) = delete;
    virtual ~Object3D() noexcept = default;

    inline const glm::mat4& coordSystemTransform() const noexcept;
    virtual void setCoordSystemTransform(const glm::mat4& newValue) noexcept;

    inline const glm::mat4& localTransform() const noexcept;
    inline const glm::mat4& localToWorldTransform() const noexcept;

  signals:
    void coordSystemChanged(const glm::mat4& newCoordSystemTransform);

    void transformChanged(const glm::mat4& newLocalToWorldTransform);

  protected:
    virtual void onSubobjectAdded(Object& object) noexcept override;
    virtual void onSubobjectRemoved(Object& object) noexcept override;

    virtual void updateLocalTransform() noexcept = 0;
    void setLocalTransform(const glm::mat4& newTransform) noexcept;

  private:
    void _updateChildsTransform() noexcept;

  private:
    glm::mat4 _coordSystemTransform;
    glm::mat4 _localTransform;
    glm::mat4 _localToWorldTransform;
  };

  inline const glm::mat4& Object3D::coordSystemTransform() const noexcept
  {
    return _coordSystemTransform;
  }

  inline const glm::mat4& Object3D::localTransform() const noexcept
  {
    return _localTransform;
  }

  inline const glm::mat4& Object3D::localToWorldTransform() const noexcept
  {
    return _localToWorldTransform;
  }
}