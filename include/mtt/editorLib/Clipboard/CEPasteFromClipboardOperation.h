#pragma once

#include <mtt/application/Clipboard/PasteFromClipboardOperation.h>
#include <mtt/editorLib/Objects/CEVisitor.h>

namespace mtt
{
  class EditorCommonData;

  class CEPasteFromClipboardOperation : public PasteFromClipboardOperation
  {
  public:
    class EnvironmentObjectHandler : public CEVisitor
    {
    public:
      EnvironmentObjectHandler(EditorCommonData& commonData);
      EnvironmentObjectHandler(const EnvironmentObjectHandler&) = delete;
      EnvironmentObjectHandler& operator = (
                                      const EnvironmentObjectHandler&) = delete;
      ~EnvironmentObjectHandler() = default;

      inline mtt::Object* targetGroup() const noexcept;

      virtual void visitEnvironmentObject(EnvironmentObject& object) override;

    protected:
      void setTargetGroup(mtt::Object* newValue);

    private:
      mtt::Object* _targetGroup;
      mtt::EnvironmentGroup* _environmentGroup;
    };

  public:
    CEPasteFromClipboardOperation(const std::string& mimeType,
                                  EditorCommonData& commonData);
    CEPasteFromClipboardOperation(
                                const CEPasteFromClipboardOperation&) = delete;
    CEPasteFromClipboardOperation& operator = (
                                const CEPasteFromClipboardOperation&) = delete;
    virtual ~CEPasteFromClipboardOperation() noexcept = default;

  protected:
    virtual Object* prepareObjectAndGetTargetGroup(
                                          Object& loadedObject,
                                          UID::ValueType mixUIDValue) override;

  private:
    EditorCommonData& _commonData;
  };

  inline mtt::Object* CEPasteFromClipboardOperation::
                          EnvironmentObjectHandler::targetGroup() const noexcept
  {
    return _targetGroup;
  }
}