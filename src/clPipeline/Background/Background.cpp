#include <mtt/clPipeline/Background/Background.h>

using namespace mtt;
using namespace mtt::clPipeline;

Background::Background(LogicalDevice& device) :
  _properties{glm::vec3(1.f),
              40.f,
              10.f},
  _luminanceSampler(PipelineResource::STATIC, device),
  _areaModificator(_properties, _luminanceSampler),
  _drawable(_properties, _luminanceSampler, device)
{
}

void Background::setLuminanceTexture(
                                std::shared_ptr<CubeTexture> texture) noexcept
{
  if ((_luminanceSampler.attachedTexture(0) != nullptr) != (texture != nullptr))
  {
    _areaModificator.reset();
    _drawable.reset();
  }

  _luminanceSampler.setAttachedTexture(std::move(texture), 0);
}
