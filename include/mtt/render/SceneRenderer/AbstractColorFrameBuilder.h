#pragma once

#include <memory>

#include <mtt/render/SceneRenderer/AbstractFrameBuilder.h>

namespace mtt
{
  class Image;  
  class LogicalDevice;

  class AbstractColorFrameBuilder : public AbstractFrameBuilder
  {
  public:
    AbstractColorFrameBuilder();
    AbstractColorFrameBuilder(const AbstractColorFrameBuilder&) = delete;
    AbstractColorFrameBuilder& operator = (
                                    const AbstractColorFrameBuilder&) = delete;
    virtual ~AbstractColorFrameBuilder() noexcept = default;

    virtual std::unique_ptr<AbstractFrame> createFrame(Image & target) = 0;
  };
};
