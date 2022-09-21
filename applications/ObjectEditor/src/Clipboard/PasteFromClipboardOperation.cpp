#include <mtt/utilities/UID.h>

#include <Clipboard/CopyToClipboardOperation.h>
#include <Clipboard/PasteFromClipboardOperation.h>
#include <Objects/ObjectLoader.h>
#include <Objects/MMDObjectFactory.h>
#include <Objects/OEVisitor.h>
#include <ObjectEditorCommonData.h>

class PasteFromClipboardOperation::ObjectHandler : public OEVisitor
{
public:
  ObjectHandler(ObjectEditorCommonData& commonData) :
    _animationGroup(nullptr),
    _currentAnimation(nullptr),
    _geometryGroup(nullptr),
    _currentLOD(nullptr),
    _materialsGroup(nullptr),
    _skeletonGroup(nullptr),
    _currentSkeleton(nullptr),
    _target(nullptr)
  {
    if (commonData.scene() != nullptr)
    {
      _animationGroup = &commonData.scene()->dataRoot().animationGroup();
      _geometryGroup = &commonData.scene()->dataRoot().geometryGroup();
      _materialsGroup = &commonData.scene()->dataRoot().materialsGroup();
      _skeletonGroup = &commonData.scene()->dataRoot().skeletonGroup();
    }

    if (commonData.selectedObjects().size() == 1)
    {
      mtt::Object& selectedObject = *commonData.selectedObjects().front();
      _currentAnimation = qobject_cast<mtt::AnimationObject*>(&selectedObject);
      if (_currentAnimation == nullptr)
      {
        _currentAnimation =
                  qobject_cast<mtt::AnimationObject*>(selectedObject.parent());
      }
      _currentLOD = qobject_cast<LODObject*>(&selectedObject);
      if (_currentLOD == nullptr)
      {
        _currentLOD = qobject_cast<LODObject*>(selectedObject.parent());
      }
      _currentSkeleton = qobject_cast<mtt::SkeletonObject*>(&selectedObject);
      if (_currentSkeleton == nullptr)
      {
        _currentSkeleton =
                    qobject_cast<mtt::SkeletonObject*>(selectedObject.parent());
      }
    }
  }

  mtt::Object* target() const noexcept
  {
    return _target;
  }

  virtual void visitAnimationObject(mtt::AnimationObject& object) override
  {
    if (_animationGroup != nullptr) _target = _animationGroup;
    OEVisitor::visitAnimationObject(object);
  }

  virtual void visitAnimationTrack(mtt::AnimationTrack& object) override
  {
    if(_currentAnimation != nullptr) _target = _currentAnimation;
    OEVisitor::visitAnimationTrack(object);
  }

  virtual void visitLODObject(LODObject& object) override
  {
    if (_geometryGroup != nullptr) _target = _geometryGroup;
    OEVisitor::visitLODObject(object);
  }

  virtual void visitMaterialObject(MaterialObject& object) override
  {
    if (_materialsGroup != nullptr) _target = _materialsGroup;
    OEVisitor::visitMaterialObject(object);
  }

  virtual void visitMeshObject(MeshObject& object) override
  {
    if (_currentLOD != nullptr) _target = _currentLOD;
    OEVisitor::visitMeshObject(object);
  }

  virtual void visitSkeletonObject(mtt::SkeletonObject& object) override
  {
    if (_currentSkeleton != nullptr) _target = _currentSkeleton;
    else if(_skeletonGroup != nullptr) _target = _skeletonGroup;
    OEVisitor::visitSkeletonObject(object);
  }

private:
  mtt::AnimationGroup* _animationGroup;
  mtt::AnimationObject* _currentAnimation;
  GeometryGroup* _geometryGroup;
  LODObject* _currentLOD;
  MaterialsGroup* _materialsGroup;
  mtt::SkeletonGroup* _skeletonGroup;
  mtt::SkeletonObject* _currentSkeleton;

  mtt::Object* _target;
};


PasteFromClipboardOperation::PasteFromClipboardOperation(
                                          ObjectEditorCommonData& commonData) :
  mtt::CEPasteFromClipboardOperation(commonData),
  _commonData(commonData)
{
  std::unique_ptr<ObjectLoader> loader(new ObjectLoader());
  loader->setReferenceLoadMode(ObjectLoader::SAVE_ORIGINAL_ID_IN_REFERENCE);
  addLoader(CopyToClipboardOperation::objectEditorCategoryName,
            std::move(loader),
            std::make_unique<MMDObjectFactory>());
}

mtt::Object* PasteFromClipboardOperation::prepareObjectAndGetTargetGroup(
                                                mtt::Object& loadedObject,
                                                mtt::UID::ValueType mixUIDValue)
{
  ObjectHandler handler(_commonData);
  handler.process(loadedObject);
  if(handler.target() != nullptr)return handler.target();
  return CEPasteFromClipboardOperation::prepareObjectAndGetTargetGroup(
                                                                  loadedObject,
                                                                  mixUIDValue);
}
