#include <mtt/utilities/UID.h>

#include <Objects/ObjectLoader.h>
#include <Objects/PEEObjectFactory.h>
#include <Objects/PEVisitor.h>
#include <ParticlesEditorCommonData.h>
#include <PasteFromClipboardOperation.h>

class PasteFromClipboardOperation::ObjectHandler : public PEVisitor
{
public:
  ObjectHandler(ParticlesEditorCommonData& commonData,
                mtt::UID::ValueType uidMixValue) :
    _commonData(commonData),
    _uidMixValue(uidMixValue),
    _modificatorsGroup(nullptr),
    _selectedFrame(nullptr),
    _field(nullptr),
    _target(nullptr)
  {
    if (_commonData.scene() != nullptr)
    {
      _modificatorsGroup = &_commonData.scene()->dataRoot().modificatorsGroup();
      _field = &_commonData.scene()->dataRoot().particleField();
      _animation = &_commonData.scene()->dataRoot().animation();
    }

    if (_commonData.selectedObjects().size() == 1)
    {
      mtt::Object& selectedObject = *_commonData.selectedObjects().front();
      _selectedFrame = qobject_cast<FrameObject*>(&selectedObject);
      if (_selectedFrame == nullptr)
      {
        _selectedFrame = qobject_cast<FrameObject*>(selectedObject.parent());
      }
    }
  }

  mtt::Object* target() const noexcept
  {
    return _target;
  }

  virtual void visitAnimationTrack(mtt::AnimationTrack& object) override
  {
    if(_animation != nullptr) _target = _animation;
    PEVisitor::visitAnimationTrack(object);
  }

  virtual void visitEmitterObject(EmitterObject& object) override
  {
    if (_field != nullptr) object.fieldRef().set(_field);
    PEVisitor::visitEmitterObject(object);
  }

  virtual void visitHierarhicalObject(HierarhicalObject& object) override
  {
    if (_selectedFrame != nullptr) _target = _selectedFrame;
    else if (_modificatorsGroup != nullptr) _target = _modificatorsGroup;
    PEVisitor::visitHierarhicalObject(object);
  }

  virtual void visitModificatorObject(ModificatorObject& object) override
  {
    if (_field != nullptr) object.fieldRef().set(_field);
    PEVisitor::visitModificatorObject(object);
  }

private:
  ParticlesEditorCommonData& _commonData;
  mtt::UID::ValueType _uidMixValue;
  ModificatorGroup* _modificatorsGroup;
  FrameObject* _selectedFrame;
  ParticleField* _field;
  ParticleAnimation* _animation;
  mtt::Object* _target;
};


PasteFromClipboardOperation::PasteFromClipboardOperation(
  ParticlesEditorCommonData& commonData) :
  mtt::PasteFromClipboardOperation( mimeType,
                                    std::make_unique<ObjectLoader>(),
                                    std::make_unique<PEEObjectFactory>(),
                                    &commonData),
  _commonData(commonData)
{
}

mtt::Object* PasteFromClipboardOperation::prepareObjectAndGetTargetGroup(
                                                mtt::Object& loadedObject,
                                                mtt::UID::ValueType mixUIDValue)
{
  ObjectHandler handler(_commonData, mixUIDValue);
  handler.process(loadedObject);
  return handler.target();
}
