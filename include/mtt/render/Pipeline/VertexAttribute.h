#pragma once

#include <string>

#include <vulkan/vulkan.h>

namespace mtt
{
  class Buffer;
  class CommandProducer;
  class GraphicsPipeline;
  class PlainBuffer;

  class VertexAttribute
  {
  public:
    enum DataType
    {
      FLOAT_TYPE,
      FLOAT_VEC2_TYPE,
      FLOAT_VEC3_TYPE,
      FLOAT_VEC4_TYPE,
      INT8_NORM_TYPE,
      INT8_NORM_VEC2_TYPE,
      INT8_NORM_VEC3_TYPE,
      INT8_NORM_VEC4_TYPE,
      UINT8_NORM_TYPE,
      UINT8_NORM_VEC2_TYPE,
      UINT8_NORM_VEC3_TYPE,
      UINT8_NORM_VEC4_TYPE,
      INT32_TYPE,
      INT32_VEC2_TYPE,
      INT32_VEC3_TYPE,
      INT32_VEC4_TYPE,
      UINT32_TYPE,
      UINT32_VEC2_TYPE,
      UINT32_VEC3_TYPE,
      UINT32_VEC4_TYPE,
    };

  private:
    friend class GraphicsPipeline;
    VertexAttribute(const std::string locationName, GraphicsPipeline& pipeline);

  public:
    VertexAttribute(const VertexAttribute&) = delete;
    VertexAttribute& operator = (const VertexAttribute&) = delete;
    ~VertexAttribute() noexcept = default;

    inline const std::string& locationName() const noexcept;
  
    inline Buffer* attachedBuffer() const noexcept;
    /// The buffer will be bound the next time the pipeline binds.
    inline void attachBuffer(Buffer* newBuffer) noexcept;

    /// Set format, data stride and location size for buffer's data type
    void adjustDataType(DataType dataType);

    inline VkFormat format() const noexcept;
    void setFormat(VkFormat newValue);

    inline uint32_t dataStride() const noexcept;
    void setDataStride(uint32_t newValue);

    inline uint32_t locationSize() const noexcept;
    void setLocationSize(uint32_t newValue);

    inline VkVertexInputRate inputRate() const noexcept;
    void setInputRate(VkVertexInputRate newValue);

    /// Attach data from buffer as vertex attribute
    void scheduleBindData(PlainBuffer& buffer, CommandProducer& drawProducer);
    /// Attach data from buffer as vertex attribute
    void scheduleBindData(Buffer& buffer,
                          CommandProducer& drawProducer);
    /// attached buffer will be used
    inline void scheduleBind(CommandProducer& drawProducer);

  private:
    friend class GraphicsPipeline;
    inline uint32_t location() const noexcept;
    inline void setLocation(uint32_t newValue);
    inline uint32_t binding() const noexcept;
    inline void setBinding(uint32_t newValue);

  private:
    GraphicsPipeline& _pipeline;
    std::string _locationName;

    VkFormat _format;
    uint32_t _dataStride;
    uint32_t _locationSize;
    VkVertexInputRate _inputRate;

    uint32_t _location;
    uint32_t _binding;

    Buffer* _attachedBuffer;
  };

  inline const std::string& VertexAttribute::locationName() const noexcept
  {
    return _locationName;
  }

  inline Buffer* VertexAttribute::attachedBuffer() const noexcept
  {
    return _attachedBuffer;
  }

  inline void VertexAttribute::attachBuffer(Buffer* newBuffer) noexcept
  {
    _attachedBuffer = newBuffer;
  }

  inline VkFormat VertexAttribute::format() const noexcept
  {
    return _format;
  }

  inline uint32_t VertexAttribute::dataStride() const noexcept
  {
    return _dataStride;
  }

  inline uint32_t VertexAttribute::locationSize() const noexcept
  {
    return _locationSize;
  }

  inline VkVertexInputRate VertexAttribute::inputRate() const noexcept
  {
    return _inputRate;
  }

  inline uint32_t VertexAttribute::location() const noexcept
  {
    return _location;
  }

  inline void VertexAttribute::setLocation(uint32_t newValue)
  {
    _location = newValue;
  }

  inline uint32_t VertexAttribute::binding() const noexcept
  {
    return _binding;
  }

  inline void VertexAttribute::setBinding(uint32_t newValue)
  {
    _binding = newValue;
  }

  inline void VertexAttribute::scheduleBind(CommandProducer& drawProducer)
  {
    if(_attachedBuffer != nullptr)
    {
      scheduleBindData(*_attachedBuffer, drawProducer);
    }
  }
}