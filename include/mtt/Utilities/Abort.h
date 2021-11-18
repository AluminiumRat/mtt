#pragma once

#include <mtt/Utilities/Log.h>

namespace mtt
{
  template <typename ReasonType>
  inline void Abort(ReasonType reason)
  {
    Log() << reason;
    abort();
  }
}