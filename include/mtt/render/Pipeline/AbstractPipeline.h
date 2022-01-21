#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include <mtt/render/Pipeline/PipelineResourceSet.h>
#include <mtt/render/Pipeline/ShaderModule.h>
#include <mtt/render/Lockable.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class CommandProducer;
  class LogicalDevice;

  class AbstractPipeline
  {
  public:
    AbstractPipeline(VkPipelineBindPoint bindPoint, LogicalDevice& device);
    AbstractPipeline(const AbstractPipeline&) = delete;
    AbstractPipeline& operator = (const AbstractPipeline&) = delete;
    virtual ~AbstractPipeline() noexcept = default;

  public:
    inline VkPipelineBindPoint bindPoint() const noexcept;
    inline LogicalDevice& device() const noexcept;

    void setDefine(const std::string& name, const std::string& value);
    void setDefine(const std::string& name);
    void removeDefine(const std::string& name) noexcept;

    void addResource( const std::string& bindingName,
                      PipelineResource& resource,
                      VkShaderStageFlags stageFlags);
    void removeResource(const std::string& bindingName) noexcept;

    virtual void scheduleBind(CommandProducer& drawProducer);

    void forceBuild();

  protected:
    inline size_t shaderNumber() const noexcept;
    inline ShaderModule& shader(size_t shaderIndex) noexcept;
    inline const ShaderModule& shader(size_t shaderIndex) const noexcept;
    virtual void addShader(std::unique_ptr<ShaderModule> shaderModule);
    virtual std::unique_ptr<ShaderModule>
                              removeShader(ShaderModule& shaderModule) noexcept;

    friend class PipelineResourceSet;
    inline void invalidate() noexcept;

    virtual void rebuild();
    virtual VkPipeline createPipeline(
              const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages,
              VkPipelineLayout pipelineLayout) = 0;

  private:
    bool _shadersChanged() const noexcept;
    void _adjustDescriptorSets();
    void _adjustResourceLocations();

  private:
    class Handle : public Lockable
    {
    public:
      VkPipeline value = VK_NULL_HANDLE;
      VkDevice device = VK_NULL_HANDLE;

      Handle() noexcept = default;
      Handle(const Handle&) = delete;
      Handle& operator = (const Handle&) = delete;
      virtual ~Handle() noexcept;
    };
    Ref<Handle> _handle;
  
    VkPipelineBindPoint _bindPoint;
    LogicalDevice& _device;

    struct ShaderRecord
    {
      std::unique_ptr<ShaderModule> shaderModule;
      VkShaderStageFlagBits stage = VK_SHADER_STAGE_VERTEX_BIT;
      size_t lastRevision = 0;
  
      ShaderRecord() noexcept = default;
      inline ShaderRecord(ShaderRecord&& other) noexcept;
      ~ShaderRecord() noexcept = default;
      inline ShaderRecord& operator = (ShaderRecord&& other) noexcept;
    };
    using Shaders = std::vector<ShaderRecord>;
    Shaders _shaders;

    PipelineResourceSet _resourceSet;

    using Defines = std::map<std::string, std::string>;
    Defines _defines;
  };

  inline void AbstractPipeline::invalidate() noexcept
  {
    _handle.reset();
  }

  inline VkPipelineBindPoint AbstractPipeline::bindPoint() const noexcept
  {
    return _bindPoint;
  }

  inline LogicalDevice& AbstractPipeline::device() const noexcept
  {
    return _device;
  }

  inline size_t AbstractPipeline::shaderNumber() const noexcept
  {
    return _shaders.size();
  }

  inline ShaderModule& AbstractPipeline::shader(size_t shaderIndex) noexcept
  {
    return *_shaders[shaderIndex].shaderModule;
  }

  inline const ShaderModule& AbstractPipeline::shader(
                                              size_t shaderIndex) const noexcept
  {
    return *_shaders[shaderIndex].shaderModule;
  }

  inline AbstractPipeline::ShaderRecord::ShaderRecord(
                                                ShaderRecord&& other) noexcept :
    stage(other.stage),
    lastRevision(other.lastRevision)
  {
    shaderModule.swap(other.shaderModule);
  }

  inline AbstractPipeline::ShaderRecord&
      AbstractPipeline::ShaderRecord::operator = (ShaderRecord&& other) noexcept
  {
    shaderModule.swap(other.shaderModule);
    stage = other.stage;
    lastRevision = other.lastRevision;
    return *this;
  }
}