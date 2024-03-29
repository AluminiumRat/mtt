#pragma once

#include <memory>
#include <string>
#include <vector>

#include <QtCore/QByteArray>

#include <mtt/application/Scene/ObjectFactory.h>
#include <mtt/application/Scene/ObjectLoader.h>

namespace mtt
{
  class CommonEditData;
  class DataStream;

  class PasteFromClipboardOperation
  {
  public:
    PasteFromClipboardOperation(const std::string& mimeType,
                                CommonEditData* commonData);
    PasteFromClipboardOperation(const PasteFromClipboardOperation&) = delete;
    PasteFromClipboardOperation& operator = (
                                  const PasteFromClipboardOperation&) = delete;
    virtual ~PasteFromClipboardOperation() noexcept = default;

    inline const std::string& mimeType() const noexcept;

    virtual bool isPasteAvailable() const noexcept;
    virtual void pasteFromClipboard();

  protected:
    /// objectLoader and objectFactory must not be nullptr
    void addLoader( std::string categoryName,
                    std::unique_ptr<ObjectLoader> objectLoader,
                    std::unique_ptr<ObjectFactory> objectFactory);

    virtual mtt::Object* prepareObjectAndGetTargetGroup(
                                                Object& loadedObject,
                                                UID::ValueType mixUIDValue) = 0;

  private:
    struct LoaderRecord
    {
      std::string categoryName;
      std::unique_ptr<ObjectLoader> loader;
      std::unique_ptr<ObjectFactory> factory;

      LoaderRecord() = default;
      LoaderRecord(const LoaderRecord&) = delete;
      LoaderRecord(LoaderRecord&&) = default;
      LoaderRecord& operator = (const LoaderRecord&) = delete;
      LoaderRecord& operator = (LoaderRecord&&) = default;
      ~LoaderRecord() noexcept = default;
    };

  private:
    QByteArray _getClipboardData() const;
    LoaderRecord& _getLoader(DataStream& inStream);

  private:
    std::string _mimeType;
    std::vector<LoaderRecord> _loaders;
    CommonEditData* _commonData;
  };

  inline const std::string&
                          PasteFromClipboardOperation::mimeType() const noexcept
  {
    return _mimeType;
  }
}