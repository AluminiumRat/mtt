#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include <mtt/Render/Pipeline/PipelineResource.h>
#include <mtt/Render/Utilities/DescriptorCounter.h>
#include <mtt/Render/Lockable.h>
#include <mtt/Render/Ref.h>

namespace mtt
{
  class AbstractPipeline;
  class CommandProducer;
  class LogicalDevice;

  class PipelineResourceSet
  {
  public:
    struct Binding
    {
      PipelineResource* resource;
      VkShaderStageFlags stageFlags;
      std::string bindingName;
      size_t lastRevision;
    };

  public:
    PipelineResourceSet(AbstractPipeline& pipeline,
                        VkPipelineBindPoint bindPoint,
                        LogicalDevice& device);
    PipelineResourceSet(const PipelineResourceSet&) = delete;
    PipelineResourceSet& operator = (const PipelineResourceSet&) = delete;
    ~PipelineResourceSet() noexcept = default;

    /// Rebuilds as needs.
    inline VkPipelineLayout pipelineLayoutHandle() noexcept;

    void addResource( PipelineResource& resource,
                      VkShaderStageFlags stageFlags,
                      const std::string& bindingName);
    inline size_t resourceNumber() noexcept;
    inline const Binding& resource(size_t index) const noexcept;
    inline void removeResource(size_t index) noexcept;

    inline size_t staticResourceNumber() noexcept;
    inline const Binding& staticResource(size_t index) const noexcept;

    inline size_t volatileResourceNumber() const noexcept;
    inline const Binding& volatileResource(size_t index) const noexcept;

    void scheduleBind(CommandProducer& drawProducer);

  private:
    class LayoutHandles : public Lockable
    {
    public:
      VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
      VkDescriptorSetLayout commonDescriptorSetLayout = VK_NULL_HANDLE;
      VkDescriptorSetLayout volatileDescriptorSetLayout = VK_NULL_HANDLE;
      VkDevice device = VK_NULL_HANDLE;

      LayoutHandles() = default;
      LayoutHandles(const LayoutHandles&) = delete;
      LayoutHandles& operator = (const LayoutHandles&) = delete;
      virtual ~LayoutHandles();
    };

    class DescriptorHandles : public Lockable
    {
    public:
      VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
      VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
      VkDevice device = VK_NULL_HANDLE;

      DescriptorHandles() = default;
      DescriptorHandles(const DescriptorHandles&) = delete;
      DescriptorHandles& operator = (const DescriptorHandles&) = delete;
      virtual ~DescriptorHandles();
    };

  private:
    void _removeStaticResource(size_t index) noexcept;
    void _removeVolatileResource(size_t index) noexcept;
    void _invalidate() noexcept;
    void _buildLayouts();
    void _buildCommonSetLayout(LayoutHandles& target);
    void _buildVolatileSetLayout(LayoutHandles& target);
    void _buildDescriptors();
    bool _needUpdateDescriptors() const noexcept;

  public:
    AbstractPipeline& _pipeline;
    LogicalDevice& _device;
    VkPipelineBindPoint _bindPoint;

    Ref<LayoutHandles> _layoutHandles;
    Ref<DescriptorHandles> _descriptorHandles;

    using Bindings = std::vector<Binding>;
    Bindings _staticResources;

    Bindings _volatileResources;
    DescriptorCounter _volatileDescriptorNumber;
  };

  inline VkPipelineLayout PipelineResourceSet::pipelineLayoutHandle() noexcept
  {
    if(_layoutHandles == nullptr) _buildLayouts();
    return _layoutHandles->pipelineLayout;
  }

  inline size_t PipelineResourceSet::resourceNumber() noexcept
  {
    return _staticResources.size() + _volatileResources.size();
  }

  inline const PipelineResourceSet::Binding&
                      PipelineResourceSet::resource(size_t index) const noexcept
  {
    if(index < _staticResources.size()) return _staticResources[index];
    else return _volatileResources[index - _staticResources.size()];
  }

  inline void PipelineResourceSet::removeResource(size_t index) noexcept
  {
    if (index < _staticResources.size()) _removeStaticResource(index);
    else _removeVolatileResource(index - _staticResources.size());
  }

  inline size_t PipelineResourceSet::staticResourceNumber() noexcept
  {
    return _staticResources.size();
  }

  inline const PipelineResourceSet::Binding&
                PipelineResourceSet::staticResource(size_t index) const noexcept
  {
    return _staticResources[index];
  }

  inline size_t PipelineResourceSet::volatileResourceNumber() const noexcept
  {
    return _volatileResources.size();
  }

  inline const PipelineResourceSet::Binding&
              PipelineResourceSet::volatileResource(size_t index) const noexcept
  {
    return _volatileResources[index];
  }
}