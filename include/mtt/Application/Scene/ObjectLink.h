#pragma once

#include <mtt/Application/Scene/Object.h>
#include <mtt/Utilities/UID.h>

namespace mtt
{
  class ObjectRefBase
  {
  public:
    ObjectRefBase(const UID& referencedId);
    ObjectRefBase(const ObjectRefBase&) = delete;
    ObjectRefBase& operator = (const ObjectRefBase&) = delete;
    virtual ~ObjectRefBase() = default;

    inline Object* get() const noexcept;
    inline const UID& referencedId() const noexcept;

    /// Refrenced id is valid
    inline bool isValid() const noexcept;
    /// Refrenced id is valid and referenced ptr is not null
    inline bool isActive() const noexcept;

    inline bool operator == (nullptr_t) const noexcept;
    inline bool operator != (nullptr_t) const noexcept;

  protected:
    inline void addToObserver(Object& observer);
    inline void removeFromObserver(Object& observer) noexcept;

  protected:
    virtual void link(Object& referenced) = 0;
    virtual void unlink(Object& referenced) noexcept = 0;

  private:
    friend class Object;
    void setReferencedPtr(Object* referenced);

  private:
    Object* _referencedPtr;
    UID _referencedId;
  };

  template <typename Referenced>
  class ObjectRef : public ObjectRefBase
  {
  public:
    /// referencedId should belong to an object of class Referenced or
    /// should be invalid
    inline ObjectRef(const UID& referencedId);
    ObjectRef(const ObjectRefBase&) = delete;
    ObjectRef& operator = (const ObjectRefBase&) = delete;
    virtual ~ObjectRef() = default;
  
    inline Referenced* get() const noexcept;
  };

  template <typename Referenced,
            typename Observer,
            void(Observer::*linkPtr)(Referenced&),
            void(Observer::*unlinkPtr)(Referenced&) noexcept>
  class ObjectLink : public ObjectRef<Referenced>
  {
  public:
    /// referencedId should belong to an object of class Referenced or
    /// should be invalid
    ObjectLink(const UID& referencedId, Observer& observer) :
      ObjectRef<Referenced>(referencedId),
      _observer(observer)
    {
      ObjectRef<Referenced>::addToObserver(_observer);
    }

    ObjectLink(const ObjectLink&) = delete;
    ObjectLink& operator = (const ObjectLink&) = delete;
    virtual ~ObjectLink()
    {
      ObjectRef<Referenced>::removeFromObserver(_observer);
    }

  protected:
    virtual void link(Object& referenced) override
    {
      (_observer.*linkPtr)(static_cast<Referenced&>(referenced));
    }

    virtual void unlink(Object& referenced) noexcept override
    {
      (_observer.*unlinkPtr)(static_cast<Referenced&>(referenced));
    }
  
  private:
    Observer& _observer;
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

  inline void ObjectRefBase::addToObserver(Object& observer)
  {
    observer.addLink(*this);
  }

  inline void ObjectRefBase::removeFromObserver(Object& observer) noexcept
  {
    observer.removeLink(*this);
  }

  template <typename Referenced>
  inline ObjectRef<Referenced>::ObjectRef(const UID& referencedId) :
    ObjectRefBase(referencedId)
  {
  }

  template <typename Referenced>
  inline Referenced* ObjectRef<Referenced>::get() const noexcept
  {
    return static_cast<Referenced*>(ObjectRefBase::get());
  }
}