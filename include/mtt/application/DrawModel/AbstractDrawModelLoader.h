#pragma once

namespace mtt
{
  class MasterDrawModel;

  class AbstractDrawModelLoader
  {
  public:
    virtual void load(MasterDrawModel& model) = 0;
  };
}