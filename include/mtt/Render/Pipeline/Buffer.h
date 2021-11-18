#pragma once

#include <mtt/Render/Pipeline/PipelineResource.h>
#include <mtt/Render/Pipeline/VertexAttribute.h>
#include <mtt/Render/Pipeline/VertexIndices.h>
#include <mtt/Render/Lockable.h>
#include <mtt/Render/PlainBuffer.h>
#include <mtt/Render/Ref.h>
#include <mtt/Utilities/Abort.h>

namespace mtt
{
  class Buffer : public PipelineResource
  {
  public:
    enum Usage
    {
      VERTEX_BUFFER,
      INDICES_BUFFER,
      UNIFORM_BUFFER
    };

  public:
    Buffer(LogicalDevice& device, Usage usage) noexcept;
    Buffer(const Buffer&) = delete;
    Buffer& operator = (const Buffer&) = delete;
    virtual ~Buffer()  noexcept = default;

    virtual void prepareToBind(CommandProducer& drawProducer) override;
    virtual void updateDescriptor(VkDescriptorSet descriptorSet,
                                  uint32_t binding,
                                  CommandProducer& drawProducer,
                                  LogicalDevice& device) override;
    virtual size_t revision() const noexcept override;

    void makeEmpty(size_t size);
    void setData(const void* data, size_t dataSize);

    inline Usage usage() const noexcept;
    inline size_t size() const noexcept;

    inline void scheduleBindData( VertexAttribute& attribute,
                                  CommandProducer& drawProducer);

    inline void scheduleBindData( VertexIndices& indices,
                                  CommandProducer& drawProducer);

  protected:
    inline void incrementRevision() noexcept;

  private:
    Ref<PlainBuffer> _buildBuffer(const void* data, size_t dataSize);

  private:
    LogicalDevice& _device;
    Usage _usage;
    size_t _size;
    Ref<PlainBuffer> _plainBuffer;

    size_t _revision;
  };

  inline Buffer::Usage Buffer::usage() const noexcept
  {
    return _usage;
  }

  inline size_t Buffer::size() const noexcept
  {
    return _size;
  }

  inline void Buffer::scheduleBindData( VertexAttribute& attribute,
                                        CommandProducer& drawProducer)
  {
    if (_plainBuffer == nullptr) Abort("Buffer::scheduleBindData: main buffer is null.");
    attribute.scheduleBindData(*_plainBuffer, drawProducer);
  }

  inline void Buffer::scheduleBindData( VertexIndices& indices,
                                        CommandProducer& drawProducer)
  {
    if (_plainBuffer == nullptr) Abort("Buffer::scheduleBindData: main buffer is null.");
    indices.scheduleBindData(*_plainBuffer, drawProducer);
  }

  inline void Buffer::incrementRevision() noexcept
  {
    _revision++;
  }
}