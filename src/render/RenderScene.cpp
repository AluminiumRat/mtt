#include <mtt/render/SceneGraph/UnsortedField.h>
#include <mtt/render/RenderScene.h>

using namespace mtt;

RenderScene::RenderScene(std::unique_ptr<AbstractField> field) :
  _culledData(std::move(field))
{
  if(_culledData == nullptr) _culledData.reset(new UnsortedField());
}
