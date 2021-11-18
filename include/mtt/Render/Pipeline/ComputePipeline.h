#pragma once

#include <mtt/Render/Pipeline/AbstractPipeline.h>

namespace mtt
{
  class ComputePipeline : public AbstractPipeline
  {
  public:
    explicit ComputePipeline(LogicalDevice& device);
    ComputePipeline(const ComputePipeline&) = delete;
    ComputePipeline& operator = (const ComputePipeline&) = delete;
    virtual ~ComputePipeline() noexcept = default;

    void setShader(std::unique_ptr<ShaderModule> shaderModule);
    std::unique_ptr<ShaderModule> removeShader() noexcept;

  protected:
    virtual VkPipeline createPipeline(
              const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages,
              VkPipelineLayout pipelineLayout) override;
  };
};