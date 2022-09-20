#include <mtt/editorLib/Clipboard/CECopyToClipboardOperation.h>
#include <mtt/editorLib/Clipboard/CEPasteFromClipboardOperation.h>
#include <mtt/editorLib/Objects/CEObjectLoader.h>
#include <mtt/editorLib/Objects/EnvironmentObjectFactory.h>
#include <mtt/editorLib/EditorCommonData.h>

using namespace mtt;

CEPasteFromClipboardOperation::EnvironmentObjectHandler::
                      EnvironmentObjectHandler(EditorCommonData& commonData) :
  _targetGroup(nullptr),
  _environmentGroup(nullptr)
{
  if (commonData.scene() != nullptr)
  {
    _environmentGroup = &commonData.scene()->environmentRoot().objectsGroup();
  }
}

void CEPasteFromClipboardOperation::EnvironmentObjectHandler::
                              visitEnvironmentObject(EnvironmentObject& object)
{
  if(_environmentGroup != nullptr) setTargetGroup(_environmentGroup);
  CEVisitor::visitEnvironmentObject(object);
}

void CEPasteFromClipboardOperation::EnvironmentObjectHandler::
                                          setTargetGroup(mtt::Object* newValue)
{
  _targetGroup = newValue;
}

CEPasteFromClipboardOperation::CEPasteFromClipboardOperation(
                                                EditorCommonData& commonData) :
  PasteFromClipboardOperation(&commonData),
  _commonData(commonData)
{
  addLoader(CECopyToClipboardOperation::environmentCategoryName,
            std::make_unique<CEObjectLoader>(),
            std::make_unique<EnvironmentObjectFactory>());
}

Object* CEPasteFromClipboardOperation::prepareObjectAndGetTargetGroup(
                                                    Object& loadedObject,
                                                    UID::ValueType mixUIDValue)
{
  EnvironmentObjectHandler handler(_commonData);
  handler.process(loadedObject);
  return handler.targetGroup();
}
