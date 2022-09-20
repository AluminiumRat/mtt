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
    PasteFromClipboardOperation(CommonEditData* commonData);
    PasteFromClipboardOperation(const PasteFromClipboardOperation&) = delete;
    PasteFromClipboardOperation& operator = (
                                  const PasteFromClipboardOperation&) = delete;
    virtual ~PasteFromClipboardOperation() noexcept = default;

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
    static QByteArray _getClipboardData();
    LoaderRecord& _getLoader(DataStream& inStream);

  private:
    std::vector<LoaderRecord> _loaders;

    CommonEditData* _commonData;
  };
}