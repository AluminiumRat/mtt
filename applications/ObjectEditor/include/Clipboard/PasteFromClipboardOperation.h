#pragma once

#include <mtt/editorLib/Clipboard/CEPasteFromClipboardOperation.h>

class ObjectEditorCommonData;

class PasteFromClipboardOperation : public mtt::CEPasteFromClipboardOperation
{
public:
  PasteFromClipboardOperation(ObjectEditorCommonData& commonData);
  PasteFromClipboardOperation(const PasteFromClipboardOperation&) = delete;
  PasteFromClipboardOperation& operator = (
                                  const PasteFromClipboardOperation&) = delete;
  virtual ~PasteFromClipboardOperation() noexcept = default;

private:
  virtual mtt::Object* prepareObjectAndGetTargetGroup(
                                      mtt::Object& loadedObject,
                                      mtt::UID::ValueType mixUIDValue) override;

private:
  class ObjectHandler;

private:
  ObjectEditorCommonData& _commonData;
};