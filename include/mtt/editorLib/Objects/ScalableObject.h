#pragma once

#include <glm/vec3.hpp>

#include <mtt/editorLib/Objects/MovableObject.h>

namespace mtt
{
  class ScalableObject : public MovableObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitScalableObject,
                            visitConstScalableObject,
                            MovableObject)

    Q_PROPERTY( glm::vec3 scale
                READ scale
                WRITE setScale
                NOTIFY scaleChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)
  public:
    ScalableObject( const QString& name,
                    bool canBeRenamed,
                    const UID& id = UID());
    ScalableObject(const ScalableObject&) = delete;
    ScalableObject& operator = (const ScalableObject&) = delete;
    virtual ~ScalableObject() noexcept = default;

    inline const glm::vec3& scale() const noexcept;
    virtual void setScale(const glm::vec3& newValue) noexcept;

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
}