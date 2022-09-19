#pragma once

#include <memory>

#include <QtCore/QString>

#include <mtt/application/Scene/ObjectLoader.h>

namespace mtt
{
  class CommonEditData;

  class PasteFromClipboardOperation
  {
  public:
    /// objectSaver and objectFactory must not be nullptr
    PasteFromClipboardOperation(const QString& mimeType,
                                std::unique_ptr<ObjectLoader> objectLoader,
                                std::unique_ptr<ObjectFactory> objectFactory,
                                CommonEditData* commonData);
    PasteFromClipboardOperation(const PasteFromClipboardOperation&) = delete;
    PasteFromClipboardOperation& operator = (
                                  const PasteFromClipboardOperation&) = delete;
    virtual ~PasteFromClipboardOperation() noexcept = default;

    virtual bool mimeTypeSupported(const QString& mimeType) const noexcept;

    virtual void pasteFromClipboard();

  protected:
    virtual mtt::Object* prepareObjectAndGetTargetGroup(
                                                Object& loadedObject,
                                                UID::ValueType mixUIDValue) = 0;

  private:
    QString _mimeType;
    std::unique_ptr<ObjectLoader> _objectLoader;
    std::unique_ptr<ObjectFactory> _objectFactory;
    CommonEditData* _commonData;
  };
}