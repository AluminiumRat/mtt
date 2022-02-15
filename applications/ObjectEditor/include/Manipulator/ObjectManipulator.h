#pragma once

#include <optional>

#include <QtCore/QObject>

#include <mtt/application/Manipulator/Composite3DManipulator.h>

namespace mtt
{
  class Object3D;
}

class ObjectManipulator : public QObject,
                          public mtt::Composite3DManipulator
{
  Q_OBJECT

public:
  enum Orientation
  {
    GLOBAL_ORIENTATION,
    LOCAL_ORIENTATION
  };

public:
  inline static const glm::vec4 xUnselectedColor =
                                                  glm::vec4(1.f, .1f, .1f, .2f);
  inline static const glm::vec4 xHighlihtedColor =
                                                  glm::vec4(1.f, .1f, .1f, .4f);
  inline static const glm::vec4 xActivatedColor = glm::vec4(1.f, .1f, .1f, .8f);

  inline static const glm::vec4 yUnselectedColor =
                                                  glm::vec4(.1f, 1.f, .1f, .2f);
  inline static const glm::vec4 yHighlihtedColor =
                                                  glm::vec4(.1f, 1.f, .1f, .4f);
  inline static const glm::vec4 yActivatedColor = glm::vec4(.1f, 1.f, .1f, .8f);

  inline static const glm::vec4 zUnselectedColor =
                                                  glm::vec4(.1f, .1f, 1.f, .2f);
  inline static const glm::vec4 zHighlihtedColor =
                                                  glm::vec4(.1f, .1f, 1.f, .4f);
  inline static const glm::vec4 zActivatedColor = glm::vec4(.1f, .1f, 1.f, .8f);

public:
  ObjectManipulator(mtt::Object3D& object);
  ObjectManipulator(const ObjectManipulator&) = delete;
  ObjectManipulator& operator = (const ObjectManipulator&) = delete;
  virtual ~ObjectManipulator() noexcept = default;

  inline Orientation orientation() const noexcept;
  void setOrientation(Orientation newOrientation) noexcept;

signals:
  void orientationChanged(Orientation newValue);

private:
  void _updatePosition() noexcept;

private:
  mtt::Object3D& _object;
  Orientation _orientation;
};

inline ObjectManipulator::Orientation
                                ObjectManipulator::orientation() const noexcept
{
  return _orientation;
}
