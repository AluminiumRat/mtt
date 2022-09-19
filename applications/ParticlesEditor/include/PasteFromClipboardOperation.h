#pragma once

#include <mtt/application/Clipboard/PasteFromClipboardOperation.h>

class ParticlesEditorCommonData;

class PasteFromClipboardOperation : public mtt::PasteFromClipboardOperation
{
public:
  static constexpr char mimeType[] = "application/mttPee";

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