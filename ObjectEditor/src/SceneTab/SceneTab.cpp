#include <memory>

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include <mtt/Application/EditCommands/UndoStack.h>
#include <mtt/Application/Widgets/PropertiesWidgets/BoolPropertyWidget.h>
#include <mtt/Application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>
#include <mtt/Application/Widgets/PropertiesWidgets/FloatPropertyLinearSlider.h>
#include <mtt/Application/Widgets/PropertiesWidgets/FloatPropertyProgressiveSlider.h>
#include <mtt/Application/Widgets/PropertiesWidgets/FloatPropertySpin.h>
#include <mtt/Application/Widgets/PropertiesWidgets/IntPropertySpin.h>
#include <mtt/Application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>
#include <mtt/Application/Widgets/PropertiesWidgets/RotationPropertyWidget.h>
#include <mtt/Application/Widgets/PropertiesWidgets/StringPropertyWidget.h>
#include <mtt/Application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h>
#include <mtt/Application/Widgets/ObjectsTreeView.h>
#include <mtt/Application/CommonEditData.h>
#include <mtt/Utilities/Abort.h>

#include <Objects/OEVisitor.h>
#include <SceneTab/SceneTab.h>
#include <SceneTab/CubemapWidget.h>
#include <SceneTab/TexturePropertyWidget.h>
#include <SceneTab/VertexDataWidget.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

#include <GeneratedFiles/ui_SceneTab.h>

SceneTab::SceneTab(EditorCommonData& commonEditData) :
  _ui(new Ui::sceneTab),
  _commonEditData(commonEditData),
  _treeView(nullptr),
  _propertiesPlace(nullptr)
{
  _ui->setupUi(this);

  std::unique_ptr<SceneTreeView> treeView(new SceneTreeView(&_commonEditData));
  _treeView = treeView.get();
  _ui->objectTreePlace->layout()->addWidget(_treeView);
  treeView.release();
  _treeView->setSortFilterModel(&_sortModel);
  _treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  _treeView->setDragEnabled(true);
  _treeView->setDragDropOverwriteMode(false);
  _treeView->setDragDropMode(QAbstractItemView::InternalMove);
  _treeView->setDefaultDropAction(Qt::MoveAction);

  _propertiesPlace = _ui->propertiesPlaceLayout;

  connect(&_commonEditData,
          &EditorCommonData::sceneChanged,
          this,
          &SceneTab::_setScene,
          Qt::DirectConnection);
  _setScene(_commonEditData.scene());

  connect(&_commonEditData,
          &EditorCommonData::selectedObjectsChanged,
          this,
          &SceneTab::_updatePropertiesWidgets,
          Qt::DirectConnection);
  _updatePropertiesWidgets();
}

SceneTab::~SceneTab() noexcept
{
  delete _ui;
}

void SceneTab::_setScene(EditorScene* scene)
{
  _treeView->setItemModel(nullptr);
  _itemModel.reset();

  if(scene != nullptr)
  {
    _itemModel.emplace(scene->root(), &_commonEditData.undoStack());
    _treeView->setItemModel(&_itemModel.value());
  }
}

struct PropertiesWidgetBuilder : public OEVisitor
{
  QFormLayout* layout;
  EditorCommonData* editCommonData;
  EditorScene* scene;

  virtual void visit(AmbientLightObject& object) override
  {
    OEVisitor::visit(object);

    using FloatSpin = mtt::FloatPropertySpin<AmbientLightObject>;
    FloatSpin* saturationWidget = new FloatSpin(
                                object,
                                &AmbientLightObject::saturationDistance,
                                &AmbientLightObject::setSaturationDistance,
                                &AmbientLightObject::saturationDistanceChanged,
                                editCommonData->undoStack());
    saturationWidget->setMaximum(9999.f);
    layout->addRow(QObject::tr("Saturation"), saturationWidget);

    layout->addRow( QObject::tr("Ambient map"),
                    new CubemapWidget(object.ambientMap(),
                                      editCommonData->undoStack()));
  }

  virtual void visit(AnimationObject& object) override
  {
    OEVisitor::visit(object);

    QPushButton* startButton = new QPushButton();
    layout->addRow("", startButton);
    startButton->setText(QObject::tr("Play"));
    AnimationPlayer& player = editCommonData->animationPlayer();
    QObject::connect( startButton,
                      &QPushButton::clicked,
                      [&]()
                      {
                        player.start(object);
                      });

    QPushButton* stopButton = new QPushButton();
    layout->addRow("", stopButton);
    stopButton->setText(QObject::tr("Stop"));
    QObject::connect( stopButton,
                      &QPushButton::clicked,
                      [&]()
                      {
                        player.stop();
                      });
  }

  virtual void visit(AnimationTrack& object) override
  {
    OEVisitor::visit(object);
    mtt::BoolPropertyWidget<AnimationTrack>* enabledWidget =
                      new mtt::BoolPropertyWidget<AnimationTrack>(
                                              object,
                                              &AnimationTrack::enabled,
                                              &AnimationTrack::setEnabled,
                                              &AnimationTrack::enabledChanged,
                                              editCommonData->undoStack());
    layout->addRow(QObject::tr("Enabled"), enabledWidget);

    using ReferenceWidget = mtt::ReferencePropertyWidget< AnimationTrack,
                                                            SkeletonObject>;
    ReferenceWidget* referenceWidget = new ReferenceWidget(
                                            object,
                                            &AnimationTrack::skeleton,
                                            &AnimationTrack::setSkeleton,
                                            &AnimationTrack::skeletonRefChanged,
                                            editCommonData->undoStack(),
                                            scene->root().skeletonGroup());
    layout->addRow(QObject::tr("Bone"), referenceWidget);
  }

  virtual void visit(BackgroundObject& object) override
  {
    OEVisitor::visit(object);

    mtt::BoolPropertyWidget<BackgroundObject>* lightEnabledWidget =
                      new mtt::BoolPropertyWidget<BackgroundObject>(
                                        object,
                                        &BackgroundObject::lightEnabled,
                                        &BackgroundObject::setLightEnabled,
                                        &BackgroundObject::lightEnabledChanged,
                                        editCommonData->undoStack());
    layout->addRow(QObject::tr("Light"), lightEnabledWidget);

    using ColorWidget = mtt::ColorPropertyWidget<BackgroundObject>;
    ColorWidget* albedoWidget = new ColorWidget(object,
                                                &BackgroundObject::color,
                                                &BackgroundObject::setColor,
                                                &BackgroundObject::colorChanged,
                                                editCommonData->undoStack());
    layout->addRow(QObject::tr("Color"), albedoWidget);

    using FloatSpin = mtt::FloatPropertySpin<BackgroundObject>;
    FloatSpin* luminanceWidget = new FloatSpin(
                                            object,
                                            &BackgroundObject::luminance,
                                            &BackgroundObject::setLuminance,
                                            &BackgroundObject::luminanceChanged,
                                            editCommonData->undoStack());
    layout->addRow(QObject::tr("Luminance"), luminanceWidget);

    FloatSpin* dissolutionStartWidget = new FloatSpin(
                            object,
                            &BackgroundObject::dissolutionStartDistance,
                            &BackgroundObject::setDissolutionStartDistance,
                            &BackgroundObject::dissolutionStartDistanceChanged,
                            editCommonData->undoStack());
    layout->addRow(QObject::tr("Start"), dissolutionStartWidget);

    FloatSpin* lengthWidget = new FloatSpin(
                                    object,
                                    &BackgroundObject::dissolutionLength,
                                    &BackgroundObject::setDissolutionLength,
                                    &BackgroundObject::dissolutionLengthChanged,
                                    editCommonData->undoStack());
    layout->addRow(QObject::tr("Length"), lengthWidget);

    layout->addRow( QObject::tr("Cubemap"),
                    new CubemapWidget(object.cubemap(),
                                      editCommonData->undoStack()));
  }

  virtual void visit(BoneRefObject& object) override
  {
    OEVisitor::visit(object);

    using ReferenceWidget = mtt::ReferencePropertyWidget< BoneRefObject,
                                                            SkeletonObject>;
    ReferenceWidget* referenceWidget = new ReferenceWidget(
                                                object,
                                                &BoneRefObject::bone,
                                                &BoneRefObject::setBone,
                                                &BoneRefObject::boneRefChanged,
                                                editCommonData->undoStack(),
                                                scene->root().skeletonGroup());
    layout->addRow(QObject::tr("Link"), referenceWidget);
  }

  virtual void visit(CubemapObject& object) override
  {
    OEVisitor::visit(object);

    layout->addRow( QObject::tr("Textures"),
                    new CubemapWidget(object,
                                      editCommonData->undoStack()));
  }


  virtual void visit(DirectLightObject& object) override
  {
    OEVisitor::visit(object);

    using FloatSpin = mtt::FloatPropertySpin<DirectLightObject>;
    FloatSpin* radiusWidget = new FloatSpin(object,
                                            &DirectLightObject::radius,
                                            &DirectLightObject::setRadius,
                                            &DirectLightObject::radiusChanged,
                                            editCommonData->undoStack());
    radiusWidget->setMaximum(9999.f);
    layout->addRow(QObject::tr("Radius"), radiusWidget);

    mtt::BoolPropertyWidget<DirectLightObject>* shadowEnabledWidget =
                      new mtt::BoolPropertyWidget<DirectLightObject>(
                                    object,
                                    &DirectLightObject::shadowsEnabled,
                                    &DirectLightObject::setShadowsEnabled,
                                    &DirectLightObject::shadowsEnabledChanged,
                                    editCommonData->undoStack());
    layout->addRow(QObject::tr("Shadows enabled"), shadowEnabledWidget);

    using IntSpin = mtt::IntPropertySpin<DirectLightObject, size_t>;
    IntSpin* mapsizeWidget = new IntSpin(
                                    object,
                                    &DirectLightObject::shadowmapSize,
                                    &DirectLightObject::setShadowmapSize,
                                    &DirectLightObject::shadowmapSizeChanged,
                                    editCommonData->undoStack());
    mapsizeWidget->setMinimum(1);
    mapsizeWidget->setMaximum(4096);
    layout->addRow(QObject::tr("Map size"), mapsizeWidget);

    IntSpin* cascadeWidget = new IntSpin(
                                    object,
                                    &DirectLightObject::cascadeSize,
                                    &DirectLightObject::setCascadeSize,
                                    &DirectLightObject::cascadeSizeChanged,
                                    editCommonData->undoStack());
    cascadeWidget->setMinimum(1);
    cascadeWidget->setMaximum(10);
    layout->addRow(QObject::tr("Cascade size"), cascadeWidget);

    FloatSpin* blurWidget = new FloatSpin(
                                    object,
                                    &DirectLightObject::blurSize,
                                    &DirectLightObject::setBlurSize,
                                    &DirectLightObject::blurSizeChanged,
                                    editCommonData->undoStack());
    blurWidget->setMaximum(100);
    blurWidget->setSingleStep(.01);
    layout->addRow(QObject::tr("Blur"), blurWidget);
  }

  virtual void visit(DisplayedObject& object) override
  {
    OEVisitor::visit(object);

    mtt::BoolPropertyWidget<DisplayedObject>* visibleWidget =
                      new mtt::BoolPropertyWidget<DisplayedObject>(
                                              object,
                                              &DisplayedObject::visible,
                                              &DisplayedObject::setVisible,
                                              &DisplayedObject::visibleChanged,
                                              editCommonData->undoStack());
    layout->addRow(QObject::tr("Visible"), visibleWidget);
  }

  virtual void visit(GeometryObject& object) override
  {
    OEVisitor::visit(object);
    using ReferenceWidget = mtt::ReferencePropertyWidget< GeometryObject,
                                                            SkeletonObject>;
    ReferenceWidget* referenceWidget = new ReferenceWidget(
                                            object,
                                            &GeometryObject::skeleton,
                                            &GeometryObject::setSkeleton,
                                            &GeometryObject::skeletonRefChanged,
                                            editCommonData->undoStack(),
                                            scene->root().skeletonGroup());
    layout->addRow(QObject::tr("Bone"), referenceWidget);
  }

  virtual void visit(LODObject& object) override
  {
    OEVisitor::visit(object);
    using FloatSpin = mtt::FloatPropertySpin<LODObject>;
    FloatSpin* minDistanceWidget = new FloatSpin(
                                              object,
                                              &LODObject::minMppx,
                                              &LODObject::setMinMppx,
                                              &LODObject::minMppxChanged,
                                              editCommonData->undoStack());
    layout->addRow(QObject::tr("Min mppx"), minDistanceWidget);
    minDistanceWidget->setMaximum(10000.f);

    FloatSpin* maxDistanceWidget = new FloatSpin(
                                              object,
                                              &LODObject::maxMppx,
                                              &LODObject::setMaxMppx,
                                              &LODObject::maxMppxChanged,
                                              editCommonData->undoStack());
    layout->addRow(QObject::tr("Max mppx"), maxDistanceWidget);
    maxDistanceWidget->setMaximum(10000.f);
  }

  virtual void visit(MaterialObject& object) override
  {
    OEVisitor::visit(object);
    using ColorWidget = mtt::ColorPropertyWidget<MaterialObject>;
    ColorWidget* albedoWidget = new ColorWidget(object,
                                                &MaterialObject::albedo,
                                                &MaterialObject::setAlbedo,
                                                &MaterialObject::albedoChanged,
                                                editCommonData->undoStack());
    layout->addRow(QObject::tr("Albedo"), albedoWidget);

    using TextureWidget = TexturePropertyWidget<MaterialObject>;
    TextureWidget* albedoTextureWidget =
                        new TextureWidget(object,
                                          &MaterialObject::albedoTexture,
                                          &MaterialObject::setAlbedoTexture,
                                          &MaterialObject::albedoTextureChanged,
                                          editCommonData->undoStack());
    layout->addRow(QObject::tr("Albedo texture"), albedoTextureWidget);

    mtt::BoolPropertyWidget<MaterialObject>* useAlphaWidget =
                      new mtt::BoolPropertyWidget<MaterialObject>(
                              object,
                              &MaterialObject::useAlphaFromAlbedoTexture,
                              &MaterialObject::setUseAlphaFromAlbedoTexture,
                              &MaterialObject::useAlphaFromAlbedoTextureChanged,
                              editCommonData->undoStack());
    layout->addRow(QObject::tr("Use alpha"), useAlphaWidget);

    TextureWidget* opaqueTextureWidget =
                        new TextureWidget(object,
                                          &MaterialObject::opaqueTexture,
                                          &MaterialObject::setOpaqueTexture,
                                          &MaterialObject::opaqueTextureChanged,
                                          editCommonData->undoStack());
    layout->addRow(QObject::tr("Opaque texture"), opaqueTextureWidget);

    using FloatSlider = mtt::FloatPropertyLinearSlider<MaterialObject>;
    FloatSlider* roughnessWidget = new FloatSlider(
                                              object,
                                              &MaterialObject::roughness,
                                              &MaterialObject::setRoughness,
                                              &MaterialObject::roughnessChanged,
                                              editCommonData->undoStack());
    layout->addRow(QObject::tr("Roughness"), roughnessWidget);

    using FloatSlider = mtt::FloatPropertyLinearSlider<MaterialObject>;
    FloatSlider* specularStrengthWidget = new FloatSlider(
                                      object,
                                      &MaterialObject::specularStrength,
                                      &MaterialObject::setSpecularStrength,
                                      &MaterialObject::specularStrengthChanged,
                                      editCommonData->undoStack());
    layout->addRow(QObject::tr("Specular strength"), specularStrengthWidget);

    TextureWidget* specularTextureWidget =
                      new TextureWidget(object,
                                        &MaterialObject::specularTexture,
                                        &MaterialObject::setSpecularTexture,
                                        &MaterialObject::specularTextureChanged,
                                        editCommonData->undoStack());
    layout->addRow(QObject::tr("Specular texture"), specularTextureWidget);

    TextureWidget* normalTextureWidget =
                        new TextureWidget(object,
                                          &MaterialObject::normalTexture,
                                          &MaterialObject::setNormalTexture,
                                          &MaterialObject::normalTextureChanged,
                                          editCommonData->undoStack());
    layout->addRow(QObject::tr("Normal texture"), normalTextureWidget);

    FloatSlider* metallicWidget = new FloatSlider(
                                              object,
                                              &MaterialObject::metallic,
                                              &MaterialObject::setMetallic,
                                              &MaterialObject::metallicChanged,
                                              editCommonData->undoStack());
    layout->addRow(QObject::tr("Metallic"), metallicWidget);

    FloatSlider* opaqueWidget = new FloatSlider(
                                          object,
                                          &MaterialObject::opaqueFactor,
                                          &MaterialObject::setOpaqueFactor,
                                          &MaterialObject::opaqueFactorChanged,
                                          editCommonData->undoStack());
    layout->addRow(QObject::tr("Opaque"), opaqueWidget);

    ColorWidget* emissionColorWidget = new ColorWidget(
                                          object,
                                          &MaterialObject::emissionColor,
                                          &MaterialObject::setEmissionColor,
                                          &MaterialObject::emissionColorChanged,
                                          editCommonData->undoStack());
    layout->addRow(QObject::tr("Emission color"), emissionColorWidget);

    TextureWidget* emissionTextureWidget =
                      new TextureWidget(object,
                                        &MaterialObject::emissionTexture,
                                        &MaterialObject::setEmissionTexture,
                                        &MaterialObject::emissionTextureChanged,
                                        editCommonData->undoStack());
    layout->addRow(QObject::tr("Emission texture"), emissionTextureWidget);

    using FloatSpin = mtt::FloatPropertySpin<MaterialObject>;
    FloatSpin* emissionFactorSpin = new FloatSpin(
                                        object,
                                        &MaterialObject::emissionFactor,
                                        &MaterialObject::setEmissionFactor,
                                        &MaterialObject::emissionFactorChanged,
                                        editCommonData->undoStack());
    layout->addRow(QObject::tr("Emission factor"), emissionFactorSpin);

    using ProgressiveSlider =
                          mtt::FloatPropertyProgressiveSlider<MaterialObject>;
    ProgressiveSlider* emissionFactorWidget = new ProgressiveSlider(
                                        object,
                                        &MaterialObject::emissionFactor,
                                        &MaterialObject::setEmissionFactor,
                                        &MaterialObject::emissionFactorChanged,
                                        editCommonData->undoStack());
    layout->addRow("", emissionFactorWidget);
    emissionFactorWidget->setRange(0, 100);

    FloatSlider* reflectionFactorWidget = new FloatSlider(
                                      object,
                                      &MaterialObject::reflectionFactor,
                                      &MaterialObject::setReflectionFactor,
                                      &MaterialObject::reflectionFactorChanged,
                                      editCommonData->undoStack());
    layout->addRow(QObject::tr("Reflection factor"), reflectionFactorWidget);

    TextureWidget* reflectionTextureWidget =
                    new TextureWidget(object,
                                      &MaterialObject::reflectionTexture,
                                      &MaterialObject::setReflectionTexture,
                                      &MaterialObject::reflectionTextureChanged,
                                      editCommonData->undoStack());
    layout->addRow(QObject::tr("Reflection texture"), reflectionTextureWidget);
  }

  virtual void visit(MeshObject& object) override
  {
    OEVisitor::visit(object);
    VertexDataWidget* vertexDataWidget = new VertexDataWidget(object);
    layout->addRow(QObject::tr("Vertex data"), vertexDataWidget);
  }

  virtual void visit(mtt::Object& object) override
  {
    mtt::StringPropertyWidget<mtt::Object>* nameWidget =
                  new mtt::StringPropertyWidget<mtt::Object>(
                                                object,
                                                &mtt::Object::objectName,
                                                &mtt::Object::setObjectName,
                                                &mtt::Object::objectNameChanged,
                                                editCommonData->undoStack());
    layout->addRow(QObject::tr("Name"), nameWidget);
    OEVisitor::visit(object);
  }

  virtual void visit(LightObject& object) override
  {
    OEVisitor::visit(object);

    mtt::BoolPropertyWidget<LightObject>* enabledWidget =
                      new mtt::BoolPropertyWidget<LightObject>(
                                                  object,
                                                  &LightObject::enabled,
                                                  &LightObject::setEnabled,
                                                  &LightObject::enabledChanged,
                                                  editCommonData->undoStack());
    layout->addRow(QObject::tr("Enabled"), enabledWidget);

    using ColorWidget = mtt::ColorPropertyWidget<LightObject>;
    ColorWidget* colorWidget = new ColorWidget( object,
                                                &LightObject::color,
                                                &LightObject::setColor,
                                                &LightObject::colorChanged,
                                                editCommonData->undoStack());
    layout->addRow(QObject::tr("Color"), colorWidget);

    using FloatSpin = mtt::FloatPropertySpin<LightObject>;
    FloatSpin* illuminanceWidget = new FloatSpin(
                                          object,
                                          &LightObject::baseIlluminance,
                                          &LightObject::setBaseIlluminance,
                                          &LightObject::baseIlluminanceChanged,
                                          editCommonData->undoStack());
    illuminanceWidget->setMaximum(9.f);
    layout->addRow(QObject::tr("Power"), illuminanceWidget);

    FloatSpin* distanceWidget = new FloatSpin(
                                              object,
                                              &LightObject::distance,
                                              &LightObject::setDistance,
                                              &LightObject::distanceChanged,
                                              editCommonData->undoStack());
    distanceWidget->setMaximum(9999.f);
    layout->addRow(QObject::tr("Distance"), distanceWidget);
  }

  virtual void visit(MovableObject& object) override
  {
    visit(static_cast<Object3D&>(object));

    mtt::Vec3PropertyWidget<MovableObject>* positionWidget =
                  new mtt::Vec3PropertyWidget<MovableObject>(
                                                object,
                                                &MovableObject::position,
                                                &MovableObject::setPosition,
                                                &MovableObject::positionChanged,
                                                editCommonData->undoStack());
    layout->addRow(QObject::tr("Position"), positionWidget);

    mtt::RotationPropertyWidget<MovableObject>* rotationWidget =
              new mtt::RotationPropertyWidget<MovableObject>(
                                                object,
                                                &MovableObject::rotation,
                                                &MovableObject::setRotation,
                                                &MovableObject::rotationChanged,
                                                editCommonData->undoStack());
    layout->addRow(QObject::tr("Rotation"), rotationWidget);
  }

  virtual void visit(RotatableObject& object) override
  {
    OEVisitor::visit(object);

    mtt::RotationPropertyWidget<RotatableObject>* rotationWidget =
                new mtt::RotationPropertyWidget<RotatableObject>(
                                              object,
                                              &RotatableObject::rotation,
                                              &RotatableObject::setRotation,
                                              &RotatableObject::rotationChanged,
                                              editCommonData->undoStack());
    layout->addRow(QObject::tr("Rotation"), rotationWidget);
  }

  virtual void visit(ScalableObject& object) override
  {
    OEVisitor::visit(object);

    mtt::Vec3PropertyWidget<ScalableObject>* rotationWidget =
                    new mtt::Vec3PropertyWidget<ScalableObject>(
                                                  object,
                                                  &ScalableObject::scale,
                                                  &ScalableObject::setScale,
                                                  &ScalableObject::scaleChanged,
                                                  editCommonData->undoStack());
    layout->addRow(QObject::tr("Scale"), rotationWidget);
  }
};

void SceneTab::_updatePropertiesWidgets()
{
  QList<QWidget*> childWidgets = _ui->propertiesPlace->findChildren<QWidget*>(
                                                    QString(),
                                                    Qt::FindDirectChildrenOnly);
  for (QWidget* child : childWidgets) delete child;

  if(_commonEditData.selectedObjects().size() != 1) return;
  if(_commonEditData.scene() == nullptr) mtt::Abort("SceneTab::_updatePropertiesWidgets: scene is null");

  QFormLayout* layout(new QFormLayout());
  _propertiesPlace->addLayout(layout);
  
  PropertiesWidgetBuilder builder;
  builder.layout = layout;
  builder.editCommonData = &_commonEditData;
  builder.scene = _commonEditData.scene();
                                  
  builder.process(*_commonEditData.selectedObjects()[0]);
}
