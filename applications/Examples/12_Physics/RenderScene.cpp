#include <glm/gtx/transform.hpp>

#include <mtt/application/Application.h>

#include <RenderScene.h>

RenderScene::RenderScene() :
  ambientLight(false, true, true, mtt::Application::instance().displayDevice()),
  directLight(false, true, mtt::Application::instance().displayDevice())
{
  ambientLight.setIlluminance(glm::vec3(.5f, .5f, .5f));
  addCompositeObject(ambientLight);

  directLight.setTransformMatrix(glm::translate(glm::vec3(0.f, 0.f, 20.f)));
  directLight.setHeight(50.f);
  directLight.setDirection(glm::vec3(.5f, .5f, -2.f));
  directLight.setIlluminance(glm::vec3(2.5f, 2.5f, 2.5f));
  directLight.setShadowDistance(50.f);
  directLight.setShadowmapExtent(1024, 1024);
  directLight.setCascadeSize(3);
  directLight.setBlurSize(.1f);
  directLight.setShadowmapField(&culledData());
  addCompositeObject(directLight);
}

RenderScene::~RenderScene()
{
  removeCompositeObject(ambientLight);
  removeCompositeObject(directLight);
}
