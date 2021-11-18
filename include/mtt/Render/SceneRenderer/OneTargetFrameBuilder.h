#pragma once

#include <mtt/Render/SceneRenderer/AbstractFrameBuilder.h>

namespace mtt
{
  class Image;

  class OneTargetFrameBuilder : public AbstractFrameBuilder
  {
  public:
    class Frame : public AbstractFrame
    {
    protected:
      inline Frame( const glm::uvec2& extent,
                    OneTargetFrameBuilder& builder);

    public:
      Frame(const Frame&) = delete;
      Frame& operator = (const Frame&) = delete;
      virtual ~Frame() noexcept = default;

      virtual Image& targetImage() noexcept = 0;
      virtual const Image& targetImage() const noexcept = 0;
    };

  public:
    inline OneTargetFrameBuilder(FrameType frameType);
    OneTargetFrameBuilder(const OneTargetFrameBuilder&) = delete;
    OneTargetFrameBuilder& operator = (const OneTargetFrameBuilder&) = delete;
    virtual ~OneTargetFrameBuilder() noexcept = default;

    virtual std::unique_ptr<Frame> createFrame(const glm::uvec2& extent) = 0;
  };

  inline OneTargetFrameBuilder::OneTargetFrameBuilder(FrameType frameType) :
    AbstractFrameBuilder(frameType)
  {
  }

  inline OneTargetFrameBuilder::Frame::Frame( const glm::uvec2& extent,
                                              OneTargetFrameBuilder& builder) :
    AbstractFrame(extent, builder)
  {
  }
}