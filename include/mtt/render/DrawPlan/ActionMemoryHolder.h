#pragma once

#include <memory>

#include <mtt/render/DrawPlan/AbstractAction.h>
#include <mtt/utilities/Abort.h>

namespace mtt
{
  class ActionDestructor
  {
  public:
    ActionDestructor() noexcept = default;
    ActionDestructor(const ActionDestructor&) = default;
    ActionDestructor& operator = (const ActionDestructor&) = default;
    ~ActionDestructor() noexcept = default;

    inline void operator() (AbstractAction* action) const noexcept;
  };

  class ActionPtr : public std::unique_ptr<AbstractAction, ActionDestructor>
  {
  public:
    ActionPtr() noexcept = default;
    explicit inline ActionPtr(AbstractAction* p) noexcept;
    ActionPtr(const ActionPtr&) = delete;
    inline ActionPtr(ActionPtr&& other) noexcept;
    ActionPtr& operator = (const ActionPtr&) = delete;
    inline ActionPtr&  operator = (ActionPtr&& other) noexcept;
  };

  class ActionMemoryHolder
  {
  public:
    inline ActionMemoryHolder(size_t size);
    ActionMemoryHolder(const ActionMemoryHolder&) = delete;
    ActionMemoryHolder& operator = (const ActionMemoryHolder&) = delete;
    ~ActionMemoryHolder() noexcept = default;

    inline size_t size() const noexcept;
    inline size_t allocated() const noexcept;
    inline size_t memoryLeft() const noexcept;

    template<typename ActionType, typename... Args>
    inline ActionPtr emplace(Args&... args);

    inline void reset() noexcept;

  private:
    size_t _size;
    std::unique_ptr<char[]> _memory;
    size_t _allocated;
  };

  void ActionDestructor::operator()(AbstractAction* resource) const noexcept
  {
    resource->~AbstractAction();
  }

  ActionPtr::ActionPtr(AbstractAction* p) noexcept:
    std::unique_ptr<AbstractAction, ActionDestructor>(p)
  {
  }

  inline ActionPtr::ActionPtr(ActionPtr&& other) noexcept :
    std::unique_ptr<AbstractAction, ActionDestructor>(std::move(other))
  {
  }

  inline ActionPtr& ActionPtr::operator = (ActionPtr&& other) noexcept
  {
    std::unique_ptr<AbstractAction, ActionDestructor>::operator=(
                                                              std::move(other));
    return *this;
  }

  inline ActionMemoryHolder::ActionMemoryHolder(size_t size) :
    _size(size),
    _memory(new char[size]),
    _allocated(0)
  {
  }

  inline size_t ActionMemoryHolder::size() const noexcept
  {
    return _size;
  }

  inline size_t ActionMemoryHolder::allocated() const noexcept
  {
    return _allocated;
  }

  inline size_t ActionMemoryHolder::memoryLeft() const noexcept
  {
    return _size - _allocated;
  }

  template<typename ActionType, typename... Args>
  inline ActionPtr ActionMemoryHolder::emplace(Args&... args)
  {
    if(sizeof(ActionType) > memoryLeft()) Abort("ActionMemoryHolder::emplace: not enough memory.");
    void* placePosition = _memory.get() + _allocated;

    ActionType* newAction = new (placePosition) ActionType(args...);
    ActionPtr result(newAction);
  
    _allocated += sizeof(ActionType);

    return result;
  }

  inline void ActionMemoryHolder::reset() noexcept
  {
    _allocated = 0;
  }
}