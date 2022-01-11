#include <mtt/Render/SceneGraph/UnsortedField.h>
#include <mtt/Render/RenderScene.h>

using namespace mtt;

RenderScene::RenderScene(std::unique_ptr<AbstractField> field) :
  _culledData(std::move(field))
{
  if(_culledData == nullptr) _culledData.reset(new UnsortedField());
}
