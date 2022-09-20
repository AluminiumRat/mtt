#pragma once

#include <mtt/editorLib/Clipboard/CEPasteFromClipboardOperation.h>

class ParticlesEditorCommonData;

class PasteFromClipboardOperation : public mtt::CEPasteFromClipboardOperation
{
public:
  PasteFromClipboardOperation(ParticlesEditorCommonData& commonData);
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
  ParticlesEditorCommonData& _commonData;
};