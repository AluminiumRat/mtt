#include <mtt/Render/CommandQueue/CommandProducer.h>
#include <mtt/Render/Pipeline/Buffer.h>
#include <mtt/Render/Pipeline/GraphicsPipeline.h>
#include <mtt/Render/Pipeline/VertexAttribute.h>
#include <mtt/Render/PlainBuffer.h>

using namespace mtt;

VertexAttribute::VertexAttribute( const std::string locationName,
                                  GraphicsPipeline& pipeline) :
  _pipeline(pipeline),
  _locationName(locationName),
  _format(VK_FORMAT_R32G32B32_SFLOAT),
  _dataStride(12),
  _locationSize(1),
  _inputRate(VK_VERTEX_INPUT_RATE_VERTEX),
  _location(0),
  _binding(0),
  _attachedBuffer(nullptr)
{
}

void VertexAttribute::adjustDataType(DataType dataType)
{
  switch(dataType)
  {
  case FLOAT_TYPE:
    _format = VK_FORMAT_R32_SFLOAT;
    _dataStride = sizeof(float);
    _locationSize = 1;
    break;

  case FLOAT_VEC2_TYPE:
    _format = VK_FORMAT_R32G32_SFLOAT;
    _dataStride = 2 * sizeof(float);
    _locationSize = 1;
    break;

  case FLOAT_VEC3_TYPE:
    _format = VK_FORMAT_R32G32B32_SFLOAT;
    _dataStride = 3 * sizeof(float);
    _locationSize = 1;
    break;

  case FLOAT_VEC4_TYPE:
    _format = VK_FORMAT_R32G32B32A32_SFLOAT;
    _dataStride = 4 * sizeof(float);
    _locationSize = 1;
    break;

  case INT8_NORM_TYPE:
    _format = VK_FORMAT_R8_SNORM;
    _dataStride = 1;
    _locationSize = 1;
    break;

  case INT8_NORM_VEC2_TYPE:
    _format = VK_FORMAT_R8G8_SNORM;
    _dataStride = 2;
    _locationSize = 1;
    break;

  case INT8_NORM_VEC3_TYPE:
    _format = VK_FORMAT_R8G8B8_SNORM;
    _dataStride = 3;
    _locationSize = 1;
    break;

  case INT8_NORM_VEC4_TYPE:
    _format = VK_FORMAT_R8G8B8A8_SNORM;
    _dataStride = 4;
    _locationSize = 1;
    break;

  case UINT8_NORM_TYPE:
    _format = VK_FORMAT_R8_UNORM;
    _dataStride = 1;
    _locationSize = 1;
    break;

  case UINT8_NORM_VEC2_TYPE:
    _format = VK_FORMAT_R8G8_UNORM;
    _dataStride = 2;
    _locationSize = 1;
    break;

  case UINT8_NORM_VEC3_TYPE:
    _format = VK_FORMAT_R8G8B8_UNORM;
    _dataStride = 3;
    _locationSize = 1;
    break;

  case UINT8_NORM_VEC4_TYPE:
    _format = VK_FORMAT_R8G8B8A8_UNORM;
    _dataStride = 4;
    _locationSize = 1;
    break;

  case INT32_TYPE:
    _format = VK_FORMAT_R32_SINT;
    _dataStride = sizeof(int32_t);
    _locationSize = 1;
    break;

  case INT32_VEC2_TYPE:
    _format = VK_FORMAT_R32G32_SINT;
    _dataStride = 2 * sizeof(int32_t);
    _locationSize = 1;
    break;

  case INT32_VEC3_TYPE:
    _format = VK_FORMAT_R32G32B32_SINT;
    _dataStride = 3 * sizeof(int32_t);
    _locationSize = 1;
    break;

  case INT32_VEC4_TYPE:
    _format = VK_FORMAT_R32G32B32A32_SINT;
    _dataStride = 4 * sizeof(int32_t);
    _locationSize = 1;
    break;

  case UINT32_TYPE:
    _format = VK_FORMAT_R32_UINT;
    _dataStride = sizeof(uint32_t);
    _locationSize = 1;
    break;

  case UINT32_VEC2_TYPE:
    _format = VK_FORMAT_R32G32_UINT;
    _dataStride = 2 * sizeof(uint32_t);
    _locationSize = 1;
    break;

  case UINT32_VEC3_TYPE:
    _format = VK_FORMAT_R32G32B32_UINT;
    _dataStride = 3 * sizeof(uint32_t);
    _locationSize = 1;
    break;

  case UINT32_VEC4_TYPE:
    _format = VK_FORMAT_R32G32B32A32_UINT;
    _dataStride = 4 * sizeof(uint32_t);
    _locationSize = 1;
    break;
  }
  _pipeline.invalidate();
}

void VertexAttribute::setFormat(VkFormat newValue)
{
  _format = newValue;
  _pipeline.invalidate();
}

void VertexAttribute::setDataStride(uint32_t newValue)
{
  _dataStride = newValue;
  _pipeline.invalidate();
}

void VertexAttribute::setLocationSize(uint32_t newValue)
{
  _locationSize = newValue;
  _pipeline.invalidate();
}

void VertexAttribute::setInputRate(VkVertexInputRate newValue)
{
  _inputRate = newValue;
  _pipeline.invalidate();
}

void VertexAttribute::scheduleBindData( PlainBuffer& buffer,
                                        CommandProducer& drawProducer)
{
  VkBuffer bufferHandle = buffer.handle();
  VkDeviceSize offset =  0;
  vkCmdBindVertexBuffers( drawProducer.bufferHandle(),
                          _binding,
                          1,
                          &bufferHandle,
                          &offset);
  drawProducer.lockResource(buffer);
}

void VertexAttribute::scheduleBindData( Buffer& buffer,
                                        CommandProducer& drawProducer)
{
  buffer.scheduleBindData(*this, drawProducer);
}
