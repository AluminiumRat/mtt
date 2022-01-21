#pragma once

namespace mtt
{
  class CommandProducer;
  class FrameBuffer;

  struct DrawContext
  {
    CommandProducer& drawProducer;
    FrameBuffer* frameBuffer;
  };
};