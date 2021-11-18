#pragma once

#include <atomic>

namespace mtt
{
  class Lockable
  {
  public:
    friend class LockableReference;

    Lockable();
    Lockable(const Lockable&) = delete;
    Lockable& operator = (const Lockable&) = delete;

  protected:
    virtual ~Lockable() = default;

  private:
    inline void _incrementCounter() noexcept;
    inline int _decrementCounter() noexcept;

  private:
    std::atomic<int> _counter;
  };

  inline void Lockable::_incrementCounter() noexcept
  {
    ++_counter;
  }

  inline int Lockable::_decrementCounter() noexcept
  {
    return --_counter;
  }
}