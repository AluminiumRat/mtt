#pragma once

#include <optional>
#include <vector>

#include <mtt/Render/PlainBuffer.h>
#include <mtt/Render/Ref.h>

namespace mtt
{
  class UniformMemoryPool
  {
  public:
    struct MemoryInfo
    {
      PlainBuffer* buffer;
      size_t offset;
    };

  public:
    UniformMemoryPool(size_t initialSize, LogicalDevice& device);
    UniformMemoryPool(const UniformMemoryPool&) = delete;
    UniformMemoryPool& operator = (const UniformMemoryPool&) = delete;
    ~UniformMemoryPool() = default;

    /// Reset data and start new iteration of memory allocation
    void start();

    inline bool isStarted() const noexcept;

    /// Finalize current iteration of memory allocation and unmap buffers
    void finish() noexcept;

    template <typename DataType>
    inline MemoryInfo write(const DataType& data);
    template <typename DataType>
    inline MemoryInfo write(const std::vector<DataType>& data);

    MemoryInfo write(const char* data, size_t size);

  private:
    void _addBuffer();
    void _openBuffer(size_t bufferIndex);

  private:
    LogicalDevice& _device;
    size_t _granularity;
    size_t _bufferSize;
  
    using Buffers = std::vector<Ref<PlainBuffer>>;
    Buffers _buffers;
    size_t _currentBufferIndex;
    PlainBuffer* _currentBuffer;
    size_t _bufferCursor;

    std::optional<PlainBuffer::MemoryMapper> _mapper;
  };

  inline bool UniformMemoryPool::isStarted() const noexcept
  {
    return _mapper.has_value();
  }

  template <typename DataType>
  inline UniformMemoryPool::MemoryInfo
                    UniformMemoryPool::write(const std::vector<DataType>& data)
  {
    return write((const char*)(data.data()), sizeof(DataType) * data.size());
  }

  template <typename DataType>
  inline UniformMemoryPool::MemoryInfo UniformMemoryPool::write(
                                                          const DataType& data)
  {
    return write((const char*)(&data), sizeof(DataType));
  }
}