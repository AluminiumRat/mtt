#pragma once

#include <mtt/application/Scene/Object.h>
#include <mtt/utilities/UID.h>

namespace mtt
{
  class ObjectRefBase
  {
  public:
    ObjectRefBase(Object& observer);
    ObjectRefBase(const ObjectRefBase&) = delete;
    ObjectRefBase& operator = (const ObjectRefBase&) = delete;
    virtual ~ObjectRefBase() noexcept = default;

    inline Object* get() const noexcept;
    inline const UID& referencedId() const noexcept;
    inline void setReferencedId(const UID& newValue);

    /// Refrenced id is valid(not null)
    inline bool isValid() const noexcept;
    /// Refrenced id is valid(not null) and referenced ptr is not null
    inline bool isActive() const noexcept;

    inline bool operator == (nullptr_t) const noexcept;
    inline bool operator != (nullptr_t) const noexcept;

    inline Object& observer() const noexcept;

  protected:
    inline void addToObserver(Object& observer);
    inline void removeFromObserver(Object & observer) noexcept;
    inline void updateFromObserver() noexcept;

  protected:
    virtual void link(Object& referenced) = 0;
    virtual void unlink(Object& referenced) noexcept = 0;
    virtual void onReferenceChanged(Object* referenced) noexcept = 0;

  private:
    friend class Object;
    void setReferencedPtr(Object* referenced);

  private:
    Object& _observer;
    Object* _referencedPtr;
    UID _referencedId;
  };

  template <typename Referenced>
  class ObjectRef : public ObjectRefBase
  {
  public:
    inline ObjectRef(Object& observer);
    ObjectRef(const ObjectRefBase&) = delete;
    ObjectRef& operator = (const ObjectRefBase&) = delete;
    virtual ~ObjectRef() noexcept = default;
  
    inline Referenced* get() const noexcept;
    inline void set(Referenced* object);
  };

  /// Warning!!! onChangedPtr must be noexcept. Current signature has been made
  /// to suitable with Qt signals.
  template <typename Referenced,
            typename Observer,
            void(Observer::*linkPtr)(Referenced&),
            void(Observer::*unlinkPtr)(Referenced&) noexcept,
            void(Observer::*onChangedPtr)(Referenced*)>
  class ObjectLink : public ObjectRef<Referenced>
  {
  public:
    ObjectLink(Observer& observer) :
      ObjectRef<Referenced>(observer)
    {
      ObjectRef<Referenced>::addToObserver(observer);
    }

    ObjectLink(const ObjectLink&) = delete;
    ObjectLink& operator = (const ObjectLink&) = delete;
    virtual ~ObjectLink()
    {
      ObjectRef<Referenced>::removeFromObserver(ObjectRefBase::observer());
    }

  protected:
    virtual void link(Object& referenced) override
    {
      if (linkPtr != nullptr)
      {
        Observer& observerRef =
                              static_cast<Observer&>(ObjectRefBase::observer());
        (observerRef.*linkPtr)(static_cast<Referenced&>(referenced));
      }
    }

    virtual void unlink(Object& referenced) noexcept override
    {
      if (unlinkPtr != nullptr)
      {
        Observer& observerRef =
                              static_cast<Observer&>(ObjectRefBase::observer());
        (observerRef.*unlinkPtr)(static_cast<Referenced&>(referenced));
      }
    }

    virtual void onReferenceChanged(Object* referenced) noexcept override
    {
      if (onChangedPtr != nullptr)
      {
        Observer& observerRef =
                              static_cast<Observer&>(ObjectRefBase::observer());
        (observerRef.*onChangedPtr)(static_cast<Referenced*>(referenced));
      }
    }
  };

  inline Object* ObjectRefBase::get() const noexcept
  {
    return _referencedPtr;
  }

  inline const UID& ObjectRefBase::referencedId() const noexcept
  {
    return _referencedId;
  }

  inline bool ObjectRefBase::isValid() const noexcept
  {
    return _referencedId.isValid();
  }

  inline bool ObjectRefBase::isActive() const noexcept
  {
    return _referencedPtr != nullptr;
  }

  inline bool ObjectRefBase::operator == (nullptr_t) const noexcept
  {
    return _referencedPtr == nullptr;
  }

  inline bool ObjectRefBase::operator != (nullptr_t) const noexcept
  {
    return _referencedPtr != nullptr;
  }

  inline Object& ObjectRefBase::observer() const noexcept
  {
    return _observer;
  }

  inline void ObjectRefBase::setReferencedId(const UID& newValue)
  {
    _referencedId = UID();
    setReferencedPtr(nullptr);
    _referencedId = newValue;
    updateFromObserver();
  }

  inline void ObjectRefBase::updateFromObserver() noexcept
  {
    _observer.updateLink(*this);
  }

  inline void ObjectRefBase::addToObserver(Object& observer)
  {
    observer.addLink(*this);
  }

  inline void ObjectRefBase::removeFromObserver(Object& observer) noexcept
  {
    observer.removeLink(*this);
  }

  template <typename Referenced>
  inline ObjectRef<Referenced>::ObjectRef(Object& observer) :
    ObjectRefBase(observer)
  {
  }

  template <typename Referenced>
  inline Referenced* ObjectRef<Referenced>::get() const noexcept
  {
    return static_cast<Referenced*>(ObjectRefBase::get());
  }

  template <typename Referenced>
  inline void ObjectRef<Referenced>::set(Referenced* object)
  {
    if(object == nullptr) setReferencedId(UID());
    else setReferencedId(object->id());
  }
}