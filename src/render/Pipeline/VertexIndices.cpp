#include <mtt/render/CommandQueue/CommandProducer.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/render/Pipeline/VertexIndices.h>
#include <mtt/render/PlainBuffer.h>

using namespace mtt;

VertexIndices::VertexIndices() :
  _type(VK_INDEX_TYPE_UINT16),
  _attachedBuffer(nullptr)
{
}

void VertexIndices::scheduleBindData( PlainBuffer& buffer,
                                      CommandProducer& drawProducer)
{
  VkBuffer bufferHandle = buffer.handle();
  vkCmdBindIndexBuffer( drawProducer.bufferHandle(),
                        buffer.handle(),
                        0,
                        _type);
  drawProducer.lockResource(buffer);
}

void VertexIndices::scheduleBindData( Buffer& buffer,
                                      CommandProducer& drawProducer)
{
  buffer.scheduleBindData(*this, drawProducer);
}
