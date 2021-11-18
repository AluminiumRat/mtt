#include <mtt/Render/Pipeline/ComputePipeline.h>
#include <mtt/Render/LogicalDevice.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

ComputePipeline::ComputePipeline(LogicalDevice& device) :
  AbstractPipeline(VK_PIPELINE_BIND_POINT_COMPUTE, device)
{
}

void ComputePipeline::setShader(std::unique_ptr<ShaderModule> shaderModule)
{
  if(shaderNumber() != 0) removeShader();
  addShader(std::move(shaderModule));
}

std::unique_ptr<ShaderModule> ComputePipeline::removeShader() noexcept
{
  if(shaderNumber() == 0) Abort("ComputePipeline::removeShader: shader is not setted.");
  return AbstractPipeline::removeShader(shader(0));
}

VkPipeline ComputePipeline::createPipeline(
              const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages,
              VkPipelineLayout pipelineLayout)
{
  if(shaderStages.size() == 0) Abort("ComputePipeline::createPipeline: shader is not setted.");

  VkComputePipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
  pipelineInfo.stage = shaderStages[0];
  pipelineInfo.layout = pipelineLayout;

  VkPipeline result = VK_NULL_HANDLE;
  if(vkCreateComputePipelines(device().handle(),
                              VK_NULL_HANDLE,
                              1,
                              &pipelineInfo,
                              nullptr,
                              &result) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create graphics pipeline.");
  }

  return result;
}
