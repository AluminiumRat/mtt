#include <mtt/DrawImpl/Lighting/ShadowmapBlurFilter.h>

using namespace mtt;

ShadowmapBlurFilter::ShadowmapBlurFilter( VkFormat targetFormat,
                                          VkImageLayout targetLayout,
                                          LogicalDevice& device) :
  AbstractFilter( targetFormat,
                  targetLayout,
                  device),
  _blurParameters{glm::ivec2(1, 0), // shift
                  1.f}              // weight
{
  pipeline().addResource( "blurParamUniformBinding",
                          _parametersUniform,
                          VK_SHADER_STAGE_FRAGMENT_BIT);
  _parametersUniform.setValuePtr(&_blurParameters);

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  device));
  fragmentShader->newFragment().loadFromFile("DrawImpl/shadowmapBlur.frag");
  pipeline().addShader(std::move(fragmentShader));
}
