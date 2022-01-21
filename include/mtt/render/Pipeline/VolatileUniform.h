#pragma once

#include <mtt/render/CommandQueue/CommandProducer.h>
#include <mtt/render/Pipeline/PipelineResource.h>
#include <mtt/render/LogicalDevice.h>

namespace mtt
{
  template <typename DataType>
  class VolatileUniform : public PipelineResource
  {
  public:
    VolatileUniform() noexcept;
    VolatileUniform(const VolatileUniform&) = delete;
    VolatileUniform& operator = (const VolatileUniform&) = delete;
    virtual ~VolatileUniform() noexcept = default;

    inline virtual void prepareToBind(CommandProducer& drawProducer) override;
    inline virtual void updateDescriptor( VkDescriptorSet descriptorSet,
                                          uint32_t binding,
                                          CommandProducer& drawProducer,
                                          LogicalDevice& device) override;

    inline virtual size_t revision() const noexcept override;

    inline void setValuePtr(const DataType* newValuePtr) noexcept;
    inline const DataType* valuePtr() const noexcept;

  private:
    template<typename ElementType>
    inline static void _updateDescriptor(
                                        VkDescriptorSet descriptorSet,
                                        uint32_t binding,
                                        CommandProducer& drawProducer,
                                        LogicalDevice& device,
                                        const std::vector<ElementType>& value);

    template<typename ValueType>
    inline static void _updateDescriptor( VkDescriptorSet descriptorSet,
                                          uint32_t binding,
                                          CommandProducer& drawProducer,
                                          LogicalDevice& device,
                                          const ValueType& value);

  private:
    const DataType* _valuePtr;
  };

  template <typename DataType>
  VolatileUniform<DataType>::VolatileUniform() noexcept :
    PipelineResource(VOLATILE, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
    _valuePtr(nullptr)
  {
  }

  template <typename DataType>
  inline size_t VolatileUniform<DataType>::revision() const noexcept
  {
    return 0;
  }

  template <typename DataType>
  inline void VolatileUniform<DataType>::prepareToBind(
                                                  CommandProducer& drawProducer)
  {
  }

  template <typename DataType>
  inline void VolatileUniform<DataType>::setValuePtr(
                                          const DataType* newValuePtr) noexcept
  {
    _valuePtr = newValuePtr;
  }

  template <typename DataType>
  inline const DataType* VolatileUniform<DataType>::valuePtr() const noexcept
  {
    return _valuePtr;
  }

  template <typename DataType>
  inline void VolatileUniform<DataType>::updateDescriptor(
                                                VkDescriptorSet descriptorSet,
                                                uint32_t binding,
                                                CommandProducer& drawProducer,
                                                LogicalDevice& device)
  {
    if(_valuePtr == nullptr) Abort("VolatileUniform::updateDescriptor: value pointer is not initialized.");
    _updateDescriptor(descriptorSet, binding, drawProducer, device, *_valuePtr);
  }

  template <typename DataType>
  template <typename ElementType>
  inline void VolatileUniform<DataType>::_updateDescriptor(
                                          VkDescriptorSet descriptorSet,
                                          uint32_t binding,
                                          CommandProducer& drawProducer,
                                          LogicalDevice& device,
                                          const std::vector<ElementType>& value)
  {
    if(value.size() == 0) Abort("VolatileUniform::_updateDescriptor: data is empty.");;

    UniformMemoryPool::MemoryInfo dataLocation =
                                          drawProducer.writeUniformData(value);

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = dataLocation.buffer->handle();
    bufferInfo.offset = dataLocation.offset;
    bufferInfo.range = sizeof(ElementType) * value.size();

    VkWriteDescriptorSet descriptorWriteInfo{};
    descriptorWriteInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWriteInfo.dstSet = descriptorSet;
    descriptorWriteInfo.dstBinding = binding;
    descriptorWriteInfo.dstArrayElement = 0;
    descriptorWriteInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWriteInfo.descriptorCount = 1;
    descriptorWriteInfo.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets( device.handle(),
                            1,
                            &descriptorWriteInfo,
                            0,
                            nullptr);
  }

  template <typename DataType>
  template <typename ValueType>
  inline void VolatileUniform<DataType>::_updateDescriptor(
                                                VkDescriptorSet descriptorSet,
                                                uint32_t binding,
                                                CommandProducer& drawProducer,
                                                LogicalDevice& device,
                                                const ValueType& value)
  {
    UniformMemoryPool::MemoryInfo dataLocation =
                                          drawProducer.writeUniformData(value);

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = dataLocation.buffer->handle();
    bufferInfo.offset = dataLocation.offset;
    bufferInfo.range = sizeof(value);

    VkWriteDescriptorSet descriptorWriteInfo{};
    descriptorWriteInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWriteInfo.dstSet = descriptorSet;
    descriptorWriteInfo.dstBinding = binding;
    descriptorWriteInfo.dstArrayElement = 0;
    descriptorWriteInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWriteInfo.descriptorCount = 1;
    descriptorWriteInfo.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets( device.handle(),
                            1,
                            &descriptorWriteInfo,
                            0,
                            nullptr);
  }
}