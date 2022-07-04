#pragma once

#include <float.h>

#include <mtt/application/Scene/SpecialGroup.h>
#include <mtt/editorLib/Objects/DisplayedObject.h>

#include <Objects/MeshObject.h>

class LODObject : public mtt::SpecialGroup< mtt::DisplayedObject,
                                            MeshObject>
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension,
                          visitLODObject,
                          visitConstLODObject,
                          mtt::DisplayedObject)

  Q_PROPERTY( float minMppx
              READ minMppx
              WRITE setMinMppx
              NOTIFY minMppxChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float maxMppx
              READ maxMppx
              WRITE setMaxMppx
              NOTIFY maxMppxChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  LODObject(const QString& name,
            bool canBeRenamed,
            const mtt::UID& id = mtt::UID());
  LODObject(const LODObject&) = delete;
  LODObject& operator = (const LODObject&) = delete;
  virtual ~LODObject() noexcept = default;

  inline float minMppx() const noexcept;
  void setMinMppx(float newValue) noexcept;

  inline float maxMppx() const noexcept;
  void setMaxMppx(float newValue) noexcept;

  void setRange(float newMinMppx, float newmaxMppx) noexcept;

signals:
  void minMppxChanged(float newValue);
  void maxMppxChanged(float newValue);
  void rangeChanged(float minMppx, float maxMppx);

  void childAdded(MeshObject& object);
  void childRemoved(MeshObject& object);

private:
  float _minMppx;
  float _maxMppx;
};

inline float LODObject::minMppx() const noexcept
{
  return _minMppx;
}

inline float LODObject::maxMppx() const noexcept
{
  return _maxMppx;
}
