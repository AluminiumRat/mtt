#pragma once

#include <type_traits>

#include <mtt/Application/Scene/ObjectVisitor.h>
#include <mtt/Application/Scene/Object.h>

namespace mtt
{
  template <typename TargetCastClassRaw, typename VisitorClass>
  class ObjectCaster;

  template <typename TargetCastClass, typename VisitorClass = ObjectVisitor>
  TargetCastClass* object_cast(Object& object) noexcept
  {
    ObjectCaster<TargetCastClass, VisitorClass> caster;
    caster.process(object);
    return caster.result();
  }

  template <typename TargetCastClass, typename VisitorClass = ObjectVisitor>
  TargetCastClass* object_cast(Object* object) noexcept
  {
    if (object == nullptr) return nullptr;
    return object_cast<TargetCastClass, VisitorClass>(*object);
  }

  template <typename TargetCastClass, typename VisitorClass = ObjectVisitor>
  const TargetCastClass* object_cast(const Object& object) noexcept
  {
    ObjectCaster<TargetCastClass, VisitorClass> caster;
    caster.process(object);
    return caster.constResult();
  }

  template <typename TargetCastClass, typename VisitorClass = ObjectVisitor>
  const TargetCastClass* object_cast(const Object* object) noexcept
  {
    if (object == nullptr) return nullptr;
    return object_cast<TargetCastClass, VisitorClass>(*object);
  }

  template <typename TargetCastClassRaw, typename VisitorClass>
  class ObjectCaster : public VisitorClass
  {
  public:
    typedef typename std::remove_const<TargetCastClassRaw>::type TargetCastClass;

  public:
    inline ObjectCaster() :
      _result(nullptr),
      _constResult(nullptr)
    {
    }

    ObjectCaster(const ObjectCaster&) = delete;
    ObjectCaster& operator = (ObjectCaster&) = delete;
    virtual ~ObjectCaster() = default;

    inline TargetCastClass* result() const
    {
      return _result;
    }

    inline const TargetCastClass* constResult() const
    {
      return _constResult;
    }

    inline virtual void visit(TargetCastClass& object) override
    {
      _result = &object;
    }

    inline virtual void visit(const TargetCastClass& object) override
    {
      _constResult = &object;
    }

  private:
    TargetCastClass* _result;
    const TargetCastClass* _constResult;
  };
}