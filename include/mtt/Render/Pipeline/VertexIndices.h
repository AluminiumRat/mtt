#pragma once

#include <string>

#include <vulkan/vulkan.h>

namespace mtt
{
  class Buffer;
  class CommandProducer;
  class PlainBuffer;

  class VertexIndices
  {
  public:
    VertexIndices();
    VertexIndices(const VertexIndices&) = delete;
    VertexIndices& operator = (const VertexIndices&) = delete;
    ~VertexIndices() noexcept = default;

    inline VkIndexType type() const noexcept;
    /// New type will be used in the next binding.
    inline void setType(VkIndexType newType) noexcept;

    inline Buffer* attachedBuffer() const noexcept;
    /// The buffer will be bound the next time the pipeline binds.
    inline void attachBuffer(Buffer* newBuffer) noexcept;

    /// Attach data from buffer as vertex indices
    void scheduleBindData(PlainBuffer& buffer, CommandProducer& drawProducer);
    /// Attach data from buffer as vertex indices
    void scheduleBindData(Buffer& buffer,
                          CommandProducer& drawProducer);
    /// attached buffer will be used
    inline void scheduleBind(CommandProducer& drawProducer);

  private:
    VkIndexType _type;
    Buffer* _attachedBuffer;
  };

  inline VkIndexType VertexIndices::type() const noexcept
  {
    return _type;
  }

  inline void VertexIndices::setType(VkIndexType newType) noexcept
  {
    _type = newType;
  }

  inline Buffer* VertexIndices::attachedBuffer() const noexcept
  {
    return _attachedBuffer;
  }

  inline void VertexIndices::attachBuffer(Buffer* newBuffer) noexcept
  {
    _attachedBuffer = newBuffer;
  }

  inline void VertexIndices::scheduleBind(CommandProducer& drawProducer)
  {
    if (_attachedBuffer != nullptr)
    {
      scheduleBindData(*_attachedBuffer, drawProducer);
    }
  }
}