#pragma once

#include <Objects/ModificatorObject.h>

class SizeControlObject : public ModificatorObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitSizeControlObject,
                          visitConstSizeControlObject,
                          ModificatorObject)

public:
  SizeControlObject(const QString& name,
                    bool canBeRenamed,
                    const mtt::UID& id = mtt::UID());
  SizeControlObject(const SizeControlObject&) = delete;
  SizeControlObject& operator = (const SizeControlObject&) = delete;
  virtual ~SizeControlObject() noexcept = default;

  inline float startSize() const noexcept;
  void setStartSize(float newValue) noexcept;
  inline void resetStartSize() noexcept;

  inline float endSize() const noexcept;
  void setEndSize(float newValue) noexcept;
  inline void resetEndSize() noexcept;

  virtual void simulationStep(mtt::TimeRange time) override;

signals:
  void startSizeChanged(float newValue);
  void endSizeChanged(float newValue);

private:
  float _startSize;
  float _endSize;
};

inline float SizeControlObject::startSize() const noexcept
{
  return _startSize;
}

inline void SizeControlObject::resetStartSize() noexcept
{
  setStartSize(0.f);
}

inline float SizeControlObject::endSize() const noexcept
{
  return _endSize;
}

inline void SizeControlObject::resetEndSize() noexcept
{
  setEndSize(1.f);
}
