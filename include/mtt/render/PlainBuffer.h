#pragma once

#include <stdexcept>

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

#include <mtt/render/Lockable.h>
#include <mtt/utilities/Abort.h>

namespace mtt
{
  class CommandProducer;
  class LogicalDevice;

  class PlainBuffer : public Lockable
  {
  public:
    enum Usage
    {
      UPLOAD_BUFFER,
      DOWNLOAD_BUFFER,
      VERTEX_BUFFER,
      INDICES_BUFFER,
      UNIFORM_BUFFER,
      VOLATILE_UNIFORM_BUFFER
    };

    /// Map buffer to memory.
    /// Warning! Buffer should have UPLOAD_BUFFER, DOWNLOAD_BUFFER or
    /// VOLATILE_UNIFORM_BUFFER usage.
    /// Warning! Don't use multiple mappers at the same time.
    /// Warning! Don't use mapper and uploadData method at the same time.
    class MemoryMapper
    {
    public:
      MemoryMapper(PlainBuffer& buffer);
      MemoryMapper(const MemoryMapper&) = delete;
      MemoryMapper& operator = (const MemoryMapper&) = delete;
      ~MemoryMapper();

      inline void* data() const;

    private:
      PlainBuffer& _buffer;
      void* _data;
    };

  public:
    PlainBuffer(LogicalDevice& device, size_t size, Usage usage);
    PlainBuffer(const PlainBuffer&) = delete;
    PlainBuffer& operator = (const PlainBuffer&) = delete;
  protected:
    virtual ~PlainBuffer();

  public:
    inline VkBuffer handle() const;
    inline size_t size() const;
    inline Usage usage() const;

    /// Upload data from host to gpu via direct access.
    /// Warning! Buffer should have UPLOAD_BUFFER or
    /// VOLATILE_UNIFORM_BUFFER usage.
    /// Warning! Don't use mapper and uploadData method at the same time.
    void uploadData(const void* data, size_t shift, size_t dataSize);

    void copyData(PlainBuffer& srcBuffer,
                  size_t srcOffset,
                  size_t dstOffset,
                  size_t size,
                  CommandProducer& uploadProducer);

  private:
    void _cleanup() noexcept;

  private:
    LogicalDevice& _device;
    VkBuffer _handle;
    size_t _size;
    Usage _usage;

    VmaAllocation _allocation;
  };

  inline VkBuffer PlainBuffer::handle() const
  {
    return _handle;
  }

  inline size_t PlainBuffer::size() const
  {
    return _size;
  }

  inline PlainBuffer::Usage PlainBuffer::usage() const
  {
    return _usage;
  }

  inline void* PlainBuffer::MemoryMapper::data() const
  {
    return _data;
  }
}