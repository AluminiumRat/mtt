#pragma once

#include <mtt/Application/Scene/Object.h>
#include <mtt/Utilities/static_cast_unique_ptr.h>

namespace mtt
{
  template <typename BaseClass,
            typename ChildClass>
  class SpecialGroup : public BaseClass
  {
  public:
    inline explicit SpecialGroup(const UID& id = UID());
    SpecialGroup(const SpecialGroup&) = delete;
    SpecialGroup& operator = (const SpecialGroup&) = delete;
    virtual ~SpecialGroup() noexcept = default;

    inline size_t childsNumber() const noexcept;
    inline ChildClass& child(size_t childIndex) noexcept;
    inline const ChildClass& child(size_t childIndex) const noexcept;

    inline void addChild(std::unique_ptr<ChildClass> object);
    inline std::unique_ptr<ChildClass> removeChild(
                                                ChildClass& object,
                                                bool removeFromScene) noexcept;
    inline virtual bool subobjectCanBeAdded(
                              const Object& subobject) const noexcept override;
    inline virtual std::unique_ptr<Object> tryAddSubobject(
                                  std::unique_ptr<Object> object) override;
    inline virtual bool subobjectCanBeRemoved(
                          const Object& subobject) const noexcept override;
    inline virtual std::unique_ptr<Object> tryRemoveSubobject(
                                        Object& subobject,
                                        bool removeFromScene) noexcept override;

  public:
    /// This is a preparation for signal. Imlement this method in child class.
    virtual void childAdded(ChildClass& object) = 0;
    /// This is a preparation for signal. Imlement this method in child class.
    virtual void childRemoved(ChildClass& object) = 0;
  };

  template <typename BaseClass, typename ChildClass>
  inline SpecialGroup<BaseClass,
                      ChildClass>::SpecialGroup(const UID& id) :
    BaseClass(id)
  {
  }

  template <typename BaseClass, typename ChildClass>
  inline size_t SpecialGroup< BaseClass,
                              ChildClass>::childsNumber() const noexcept
  {
    return BaseClass::subobjectNumber();
  }

  template <typename BaseClass, typename ChildClass>
  inline ChildClass&
                    SpecialGroup< BaseClass,
                                  ChildClass>::child(size_t childIndex) noexcept
  {
    return static_cast<ChildClass&>(BaseClass::subobject(childIndex));
  }

  template <typename BaseClass, typename ChildClass>
  inline const ChildClass&
              SpecialGroup< BaseClass,
                            ChildClass>::child(size_t childIndex) const noexcept
  {
    return static_cast<const ChildClass&>(BaseClass::subobject(childIndex));
  }

  template <typename BaseClass, typename ChildClass>
  inline void SpecialGroup<
                      BaseClass,
                      ChildClass>::addChild(std::unique_ptr<ChildClass> object)
  {
    ChildClass& objectRef = *object;
    BaseClass::addSubobject(std::move(object));
    emit childAdded(objectRef);
  }

  template <typename BaseClass, typename ChildClass>
  inline std::unique_ptr<ChildClass>
                        SpecialGroup< BaseClass,
                                      ChildClass>::removeChild(
                                                  ChildClass& object,
                                                  bool removeFromScene) noexcept
  {
    std::unique_ptr<Object> removedObject =
                          BaseClass::removeSubobject(object, removeFromScene);
    std::unique_ptr<ChildClass> removed =
                  static_cast_unique_ptr<ChildClass>(std::move(removedObject));
    if(removed != nullptr) emit childRemoved(*removed);
    return removed;
  }

  template <typename BaseClass, typename ChildClass>
  inline bool
    SpecialGroup< BaseClass,
                  ChildClass>::subobjectCanBeAdded(
                                        const Object& subobject) const noexcept
  {
    const ChildClass* casted = qobject_cast<const ChildClass*>(&subobject);
    return casted != nullptr;
  }

  template <typename BaseClass, typename ChildClass>
  inline std::unique_ptr<Object>
                    SpecialGroup< BaseClass,
                                  ChildClass>::tryAddSubobject(
                                                std::unique_ptr<Object> object)
  {
    ChildClass* casted = qobject_cast<ChildClass*>(object.get());
    if(casted == nullptr) return std::move(object);
    addChild(static_cast_unique_ptr<ChildClass>(std::move(object)));
    return nullptr;
  }

  template <typename BaseClass, typename ChildClass>
  inline bool SpecialGroup< BaseClass,
                            ChildClass>::subobjectCanBeRemoved(
                                        const Object& subobject) const noexcept
  {
    const ChildClass* casted = qobject_cast<const ChildClass*>(&subobject);
    return casted != nullptr;
  }

  template <typename BaseClass, typename ChildClass>
  inline std::unique_ptr<Object>
                  SpecialGroup< BaseClass,
                                ChildClass>::tryRemoveSubobject(
                                                  Object& subobject,
                                                  bool removeFromScene) noexcept
  {
    ChildClass* casted = qobject_cast<ChildClass*>(&subobject);
    if(casted == nullptr) return nullptr;
    return removeChild(static_cast<ChildClass&>(subobject), removeFromScene);
  }
}