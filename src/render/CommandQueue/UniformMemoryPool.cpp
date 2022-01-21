#include <mtt/render/CommandQueue/UniformMemoryPool.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/render/PhysicalDevice.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

UniformMemoryPool::UniformMemoryPool( size_t initialSize,
                                      LogicalDevice& device) :
  _device(device),
  _granularity(device.physicalDevice().properties().
                                        limits.minUniformBufferOffsetAlignment),
  _bufferSize(initialSize),
  _currentBufferIndex(0),
  _currentBuffer(nullptr),
  _bufferCursor(0)
{
  _bufferSize += _granularity - (_bufferSize % _granularity);
}

void UniformMemoryPool::start()
{
  if(_mapper.has_value()) Abort("UniformMemoryPool::start: allocation has already started.");
  if(_buffers.size() == 0) _addBuffer();
  _openBuffer(0);
}

void UniformMemoryPool::_addBuffer()
{
  Ref<PlainBuffer> newBuffer(new PlainBuffer(
                                        _device,
                                        _bufferSize,
                                        PlainBuffer::VOLATILE_UNIFORM_BUFFER));
  _buffers.push_back(std::move(newBuffer));
}

void UniformMemoryPool::_openBuffer(size_t bufferIndex)
{
  _currentBufferIndex = bufferIndex;
  _currentBuffer = _buffers[bufferIndex].get();
  _bufferCursor = 0;
  _mapper.emplace(*_currentBuffer);
}

void UniformMemoryPool::finish() noexcept
{
  if(!_mapper.has_value()) Abort("UniformMemoryPool::finish: allocation has not started.");
  _mapper.reset();
}

UniformMemoryPool::MemoryInfo UniformMemoryPool::write( const char* data,
                                                        size_t dataSize)
{
  size_t chunkSize = dataSize;
  if(dataSize % _granularity != 0)
  {
    chunkSize += _granularity - (dataSize % _granularity);
  }

  if(!_mapper.has_value()) Abort("UniformMemoryPool::write: allocation has not started.");
  if(chunkSize > _bufferSize) Abort("UniformMemoryPool::write: the data size is too large.");

  if(_bufferCursor + chunkSize > _bufferSize)
  {
    if(_currentBufferIndex == _buffers.size() - 1) _addBuffer();
    _openBuffer(_currentBufferIndex + 1);
  }

  MemoryInfo writeInfo;
  writeInfo.buffer = _currentBuffer;
  writeInfo.offset = _bufferCursor;

  memcpy((char*)_mapper.value().data() + _bufferCursor, data, dataSize);
  _bufferCursor += chunkSize;

  return writeInfo;
}
