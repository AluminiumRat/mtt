#pragma once

#include <vulkan/vulkan.h>

namespace mtt
{
  class CommandProducer;
  class LogicalDevice;

  class PipelineResource
  {
  public:
    enum Type
    {
      STATIC,
      VOLATILE
    };

  public:
    /// The static resource will update only if resource has changed, but
    /// all resources in the set will be updated.
    /// Volatile resource will update descriptor before each use, but
    /// static resources in set will not be updated.
    inline PipelineResource(Type type,
                            VkDescriptorType descriptorType,
                            uint32_t descriptorCount) noexcept;
    PipelineResource(const PipelineResource&) = delete;
    PipelineResource& operator = (const PipelineResource&) = delete;
    virtual ~PipelineResource() noexcept = default;

    inline Type type() const noexcept;

    inline VkDescriptorType descriptorType() const noexcept;
    inline uint32_t descriptorCount() const noexcept;

    /// Called before each use.
    virtual void prepareToBind(CommandProducer& drawProducer) = 0;

    /// Called after prepareToBind and before bind if the revision was
    /// changed. Called after prepareToBind and before each bind if resource
    /// is volatile.
    virtual void updateDescriptor(VkDescriptorSet descriptorSet,
                                  uint32_t binding,
                                  CommandProducer& drawProducer,
                                  LogicalDevice& device) = 0;
  
    /// Revision is used to detect changes.
    /// You must increment revision after every resource change.
    /// Ignored if resource is volatile.
    virtual size_t revision() const noexcept = 0;
    
  private:
    Type _type;
    VkDescriptorType _descriptorType;
    uint32_t _descriptorCount;
  };

  inline PipelineResource::PipelineResource(
                                          Type type,
                                          VkDescriptorType descriptorType,
                                          uint32_t descriptorCount) noexcept :
    _type(type),
    _descriptorType(descriptorType),
    _descriptorCount(descriptorCount)
  {
  }

  inline PipelineResource::Type PipelineResource::type() const noexcept
  {
    return _type;
  }

  inline VkDescriptorType PipelineResource::descriptorType() const noexcept
  {
    return _descriptorType;
  }

  inline uint32_t PipelineResource::descriptorCount() const noexcept
  {
    return _descriptorCount;
  }
}