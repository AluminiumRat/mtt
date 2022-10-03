#include <stdexcept>

#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/Application.h>

#include <Physics/PhysicsScene.h>
#include <RenderScene.h>
#include <RenderWindow.h>
#include <UpdateWorldAction.h>
#include <World.h>

int main(int argc, char* argv[])
{
  try
  {
    VkPhysicalDeviceFeatures features{};
    mtt::Application application( argc,
                                  argv,
                                  "Physics",
                                  { 0, 0, 0 },
                                  VK_API_VERSION_1_2,
                                  features,
                                  false);
    Q_INIT_RESOURCE(shaders);

    RenderScene renderScene;
    PhysicsScene physicsScene;
    World world(glm::vec2(50.f), renderScene, physicsScene);

    // Static geometry
    world.addBox(glm::vec2(0.0f, -1.f), 0.f, glm::vec2(20.f, 2.f), 0.f);
    world.addBox(glm::vec2(-9.5f, 5.f), 0.f, glm::vec2(1.f, 10.f), 0.f);
    world.addBox(glm::vec2(9.f, 4.f), 0.f, glm::vec2(.5f, 8.f), 0.f);

    // Dynamic objects
    world.addBox(glm::vec2(0.f, 10.f), 1.f, glm::vec2(2.f, 1.f), 1.f / 1800.f);
    world.addSphere(glm::vec2(0.f, 5.f), 1.f, 1.f / 2400.f);

    RenderWindow window(world);
    window.show();

    application.workCycle.addAction(std::make_unique<UpdateWorldAction>(world),
                                    UpdateWorldAction::actionCategory,
                                    mtt::WorkCycle::PRERENDER_PRIORITY,
                                    true);

    return application.exec();
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    return 1;
  }
}
