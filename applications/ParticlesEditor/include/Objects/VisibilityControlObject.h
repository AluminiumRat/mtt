#pragma once

#include <mtt/utilities/Range.h>

#include <Objects/ModificatorObject.h>

class VisibilityControlObject : public ModificatorObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitVisibilityControlObject,
                          visitConstVisibilityControlObject,
                          ModificatorObject)

  Q_PROPERTY( mtt::Range<float> saturationRange
              READ saturationRange
              WRITE setSaturationRange
              NOTIFY saturationRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  VisibilityControlObject(const QString& name,
                          bool canBeRenamed,
                          const mtt::UID& id = mtt::UID());
  VisibilityControlObject(const VisibilityControlObject&) = delete;
  VisibilityControlObject& operator = (const VisibilityControlObject&) = delete;
  virtual ~VisibilityControlObject() noexcept = default;

  inline const mtt::Range<float>& saturationRange() const noexcept;
  void setSaturationRange(const mtt::Range<float>& newValue) noexcept;

  virtual void simulationStep(mtt::TimeRange time) override;

signals:
  void saturationRangeChanged(const mtt::Range<float>& newValue);

private:
  mtt::Range<float> _saturationRange;
};

inline const mtt::Range<float>&
                      VisibilityControlObject::saturationRange() const noexcept
{
  return _saturationRange;
}
