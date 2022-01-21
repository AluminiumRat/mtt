#pragma once

#include <mtt/utilities/Log.h>

namespace mtt
{
  template <typename ReasonType>
  inline void Abort [[noreturn]] (ReasonType reason)
  {
    Log() << reason;
    abort();
  }
}