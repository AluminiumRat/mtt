#pragma once

#include <mtt/Render/Lockable.h>

namespace mtt
{
  class LockableReference
  {
  public:
    inline LockableReference() noexcept;
    inline explicit LockableReference(Lockable* resource) noexcept;
    inline LockableReference(const LockableReference& other) noexcept;
    inline LockableReference(LockableReference&& other) noexcept;
    inline LockableReference& operator = (Lockable* resource) noexcept;
    inline LockableReference& operator = (
                                      const LockableReference& other) noexcept;
    inline LockableReference& operator = (LockableReference&& other) noexcept;
    inline ~LockableReference() noexcept;

    inline Lockable* resource() const noexcept;

    inline void reset() noexcept;

  protected:
    inline void unref() noexcept;

  private:
    Lockable* _resource;
  };

  template <typename Resource>
  class Ref : public LockableReference
  {
  public:
    inline Ref() noexcept;
    inline explicit Ref(Resource* resource) noexcept;
    template <typename OtherResourceType>
    inline Ref(const Ref<OtherResourceType>& other) noexcept;
    template <typename OtherResourceType>
    inline Ref(Ref<OtherResourceType>&& other) noexcept;
    inline Ref& operator = (Resource* resource) noexcept;
    template <typename OtherResourceType>
    inline Ref& operator = (const Ref<OtherResourceType>& other) noexcept;
    template <typename OtherResourceType>
    inline Ref& operator = (Ref<OtherResourceType>&& other) noexcept;
    ~Ref() = default;

    inline Resource* get() const noexcept;
    inline Resource& operator*() const noexcept;
    inline Resource* operator->() const noexcept;

    explicit operator bool() const noexcept;
  };

  template <typename Resource>
  inline bool operator == (const Ref<Resource>& x, nullptr_t y)
  {
    return x.get() == y;
  }

  template <typename Resource>
  inline bool operator != (const Ref<Resource>& x, nullptr_t y)
  {
    return x.get() != y;
  }

  inline LockableReference::LockableReference() noexcept:
    _resource(nullptr)
  {
  }

  inline LockableReference::LockableReference(Lockable* resource) noexcept:
    _resource(resource)
  {
    if(_resource != nullptr) _resource->_incrementCounter();
  }

  inline LockableReference::LockableReference(
                                      const LockableReference& other) noexcept:
    _resource(other._resource)
  {
    if (_resource != nullptr) _resource->_incrementCounter();
  }

  inline LockableReference::LockableReference(
                                            LockableReference&& other) noexcept:
    _resource(other._resource)
  {
    other._resource = nullptr;
  }

  inline LockableReference& LockableReference::operator = (
                                                    Lockable* resource) noexcept
  {
    unref();

    _resource = resource;
    if (_resource != nullptr) _resource->_incrementCounter();

    return *this;
  }

  inline LockableReference& LockableReference::operator = (
                                        const LockableReference& other) noexcept
  {
    unref();

    _resource = other._resource;
    if (_resource != nullptr) _resource->_incrementCounter();

    return *this;
  }

  inline LockableReference& LockableReference::operator = (
                                            LockableReference&& other) noexcept
  {
    unref();

    _resource = other._resource;
    other._resource = nullptr;

    return *this;
  }

  inline LockableReference::~LockableReference() noexcept
  {
    unref();
  }

  inline void LockableReference::reset() noexcept
  {
    unref();
  }

  inline void LockableReference::unref() noexcept
  {
    if(_resource == nullptr) return;
    int counter = _resource->_decrementCounter();
    if(counter == 0) delete _resource;
    _resource = nullptr;
  }

  inline Lockable* LockableReference::resource() const noexcept
  {
    return _resource;
  }

  template <typename Resource>
  inline Ref<Resource>::Ref() noexcept
  {
  }

  template <typename Resource>
  inline Ref<Resource>::Ref(Resource* resource) noexcept :
    LockableReference(resource)
  {
  }

  template <typename Resource>
  template <typename OtherResourceType>
  inline Ref<Resource>::Ref(const Ref<OtherResourceType>& other) noexcept :
    LockableReference(other)
  {
    static_assert(std::is_base_of<Resource, OtherResourceType>::value, "Type cast error");
  }

  template <typename Resource>
  template <typename OtherResourceType>
  inline Ref<Resource>::Ref(Ref<OtherResourceType>&& other) noexcept :
    LockableReference(other)
  {
    static_assert(std::is_base_of<Resource, OtherResourceType>::value, "Type cast error");
  }

  template <typename Resource>
  inline Ref<Resource>& Ref<Resource>::operator = (Resource* resource) noexcept
  {
    LockableReference::operator=(resource);
    return *this;
  }

  template <typename Resource>
  template <typename OtherResourceType>
  inline Ref<Resource>& Ref<Resource>::operator = (
                                  const Ref<OtherResourceType>& other) noexcept
  {
    static_assert(std::is_base_of<Resource, OtherResourceType>::value, "Type cast error");
    LockableReference::operator=(other);
    return *this;
  }

  template <typename Resource>
  template <typename OtherResourceType>
  inline Ref<Resource>& Ref<Resource>::operator = (
                                       Ref<OtherResourceType>&& other) noexcept
  {
    static_assert(std::is_base_of<Resource, OtherResourceType>::value, "Type cast error");
    LockableReference::operator=(other);
    return *this;
  }

  template <typename Resource>
  inline Resource* Ref<Resource>::get() const noexcept
  {
    return static_cast<Resource*>(resource());
  }

  template <typename Resource>
  inline Resource* Ref<Resource>::operator->() const noexcept
  {
    return static_cast<Resource*>(resource());
  }

  template <typename Resource>
  inline Resource& Ref<Resource>::operator*() const noexcept
  {
    return *static_cast<Resource*>(resource());
  }

  template <typename Resource>
  Ref<Resource>::operator bool() const noexcept
  {
    return resource() != nullptr;
  }
}