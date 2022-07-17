#include <mtt/particles/PSTEffect/PSTEffect.h>

using namespace mtt;

PSTEffect::PSTEffect( const QString& filename,
                      Texture2DLibrary* textureLibrary,
                      LogicalDevice& device) :
  _dataSource(new PSTDataSource(filename, textureLibrary, device)),
  _drawable(device),
  _currentTime(0),
  _needUpdate(true)
{
  setDrawable(&_drawable, _dataSource->boundSphere());
  _drawable.setParticleTextures(_dataSource->textures());
  _drawable.setFalloffBaseMppx(_dataSource->lodMppx());
  _drawable.setFalloffSmoothing(_dataSource->lodSmoothing());
}

void PSTEffect::setTime(TimeT newValue) noexcept
{
  if(newValue == _currentTime) return;
  _currentTime = newValue;
  _needUpdate = true;
}

void PSTEffect::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  if (_needUpdate)
  {
    std::vector<glm::vec3> positionData;
    std::vector<glm::vec2> sizeRotationData;
    std::vector<glm::vec4> albedoData;
    std::vector<glm::vec4> emissionData;
    std::vector<uint32_t> textureIndexData;
    std::vector<uint32_t> tileIndexData;
    std::vector<uint8_t> tagData;
    _dataSource->updateData(positionData,
                            sizeRotationData,
                            albedoData,
                            emissionData,
                            textureIndexData,
                            tileIndexData,
                            tagData,
                            _currentTime);
    _drawable.setData(positionData,
                      sizeRotationData,
                      albedoData,
                      emissionData,
                      textureIndexData,
                      tileIndexData,
                      tagData);
    _needUpdate = false;
  }

  SimpleDrawableNode::buildDrawActions(buildInfo);
}
