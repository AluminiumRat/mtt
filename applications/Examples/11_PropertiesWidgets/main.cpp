#include <stdexcept>

#include <glm/gtx/transform.hpp>

#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include <mtt/application/DrawModel/MasterDrawModel.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatPropertyLinearSlider.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/application/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/Application.h>
#include <mtt/clPipeline/Lighting/AmbientLight.h>
#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/clPipeline/MeshTechniques/ModelTechniquesFactory.h>
#include <mtt/clPipeline/ColorFrameBuilder.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/RenderScene.h>

class Dino :  public QObject,
              public mtt::MasterDrawModel
{
  Q_OBJECT

public:
  Dino( mtt::MeshTechniquesFactory& techniquesFactory,
        mtt::LogicalDevice& displayDevice) :
    MasterDrawModel(":/Dino.mmd", techniquesFactory, nullptr, displayDevice),
    _position(0)
  {
  }

  float position() const noexcept
  {
    return _position;
  }

  void setPosition(float newValue)
  {
    if(_position == newValue) return;
    _position = newValue;
    setTransformMatrix(glm::translate(glm::vec3(0.f, _position, 0.f)));
    emit positionChanged(newValue);
  }

signals:
  void positionChanged(float newValue);

private:
  float _position;
};

#include "main.moc"

class Scene : public mtt::RenderScene
{
public:
  mtt::clPipeline::AmbientLight ambientLight;
  mtt::clPipeline::DirectLight directLight;
  Dino dinoModel;
  mtt::MasterDrawModel fieldModel;

public:
  Scene(mtt::MeshTechniquesFactory& techniquesFactory,
        mtt::LogicalDevice& displayDevice) :
    ambientLight(true, true, true, displayDevice),
    directLight(true, true, displayDevice),
    dinoModel(techniquesFactory, displayDevice),
    fieldModel(":/crossField.mmd", techniquesFactory, nullptr, displayDevice)
  {
    ambientLight.setIlluminance(glm::vec3(.5f, .5f, .5f));

    directLight.setTransformMatrix(glm::translate(glm::vec3(0.f, 0.f, 3.f)));
    directLight.setHeight(5.f);
    directLight.setDirection(glm::vec3(1.f, 1.f, -1.f));
    directLight.setIlluminance(glm::vec3(2.5f, 2.5f, 2.5f));
    directLight.setShadowDistance(50.f);
    directLight.setShadowmapExtent(1024, 1024);
    directLight.setCascadeSize(3);
    directLight.setBlurSize(.1f);
    directLight.setShadowmapField(&culledData());

    addCompositeObject(ambientLight);
    addCompositeObject(directLight);
    addCulledDrawable(dinoModel);
    addCulledDrawable(fieldModel);
  }

  ~Scene()
  {
    removeCompositeObject(ambientLight);
    removeCompositeObject(directLight);
    removeCulledDrawable(dinoModel);
    removeCulledDrawable(fieldModel);
  }
};

void fillWindow(QWidget& window, Scene& scene, mtt::UndoStack& undoStack)
{
  QVBoxLayout* layout = new QVBoxLayout(&window);

  mtt::FloatPropertyLinearSlider<Dino>* slider =
              new mtt::FloatPropertyLinearSlider<Dino>( scene.dinoModel,
                                                        &Dino::position,
                                                        &Dino::setPosition,
                                                        &Dino::positionChanged,
                                                        undoStack);
  layout->addWidget(slider);
  slider->setRange(-10.f, 10.f);

  QDoubleSpinBox* spin = new QDoubleSpinBox;
  layout->addWidget(spin);
  spin->setMinimum(0.f);
  spin->setMaximum(15.f);
  mtt::FloatSpinConnection<Dino>* spinConnction =
                    new mtt::FloatSpinConnection<Dino>( *spin,
                                                        scene.dinoModel,
                                                        &Dino::position,
                                                        &Dino::setPosition,
                                                        &Dino::positionChanged,
                                                        undoStack);
  spinConnction->setParent(spin);

  QPushButton* undoButton = new QPushButton("Undo");
  layout->addWidget(undoButton);
  undoButton->connect(undoButton,
                      &QPushButton::clicked,
                      [&]()
                      {
                        undoStack.undoCommand();
                      });

  QPushButton* redoButton = new QPushButton("Redo");
  layout->addWidget(redoButton);
  redoButton->connect(redoButton,
                      &QPushButton::clicked,
                      [&]()
                      {
                        undoStack.redoCommand();
                      });
}

int main(int argc, char* argv[])
{
  try
  {
    VkPhysicalDeviceFeatures features{};
    mtt::Application application( argc,
                                  argv,
                                  "Mtt properties widgets example",
                                  { 0, 0, 0 },
                                  VK_API_VERSION_1_2,
                                  features,
                                  false);

    Q_INIT_RESOURCE(shaders);

    mtt::RenderWidget renderWindow;
    renderWindow.setFixedSize(800, 600);
    renderWindow.show();

    mtt::clPipeline::ModelTechniquesFactory techniquesFactory(
                                          true,
                                          false,
                                          true,
                                          false,
                                          false,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    Scene scene(techniquesFactory, application.displayDevice());

    mtt::CameraNode camera;
    camera.setPerspectiveProjection(glm::pi<float>() / 2.f, 1.33f, 0.1f, 50.f);

    renderWindow.setSource(&scene, &camera);

    mtt::clPipeline::ColorFrameBuilder colorFrameBuilder(
                                                  renderWindow.surfaceFormat(),
                                                  application.displayDevice());
    renderWindow.setFrameBuilder(&colorFrameBuilder);

    mtt::OrbitalCameraController cameraController(renderWindow, nullptr);
    cameraController.setCenterPosition(glm::vec3(0.f, 0.f, 0.5f));
    cameraController.setDistance(1.5f);
    cameraController.setYAngle(-.5f);

    mtt::UndoStack undoStack(0);
    QWidget propertiesWindow;
    fillWindow(propertiesWindow, scene, undoStack);
    propertiesWindow.show();

    return application.exec();
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    return 1;
  }
}
