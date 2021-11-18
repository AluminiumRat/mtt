#pragma once

#include <atomic>

namespace mtt
{
  class SpinLock
  {
  public:
    SpinLock() = default;
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator = (const SpinLock&) = delete;
    ~SpinLock() = default;

    inline void lock() noexcept;
    inline void unlock() noexcept;

  private:
    std::atomic_flag _flag = ATOMIC_FLAG_INIT;
  };

  inline void SpinLock::lock() noexcept
  {
    while(_flag.test_and_set(std::memory_order_acquire));
  }

  inline void SpinLock::unlock() noexcept
  {
    _flag.clear(std::memory_order_release);
  }
}