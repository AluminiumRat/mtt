#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <mtt/application/Scene/ObjectFactory.h>
#include <mtt/application/Scene/ObjectSaver.h>

namespace mtt
{
  class CopyToClipboardOperation
  {
  public:
    CopyToClipboardOperation(const std::string& mimeType);
    CopyToClipboardOperation(const CopyToClipboardOperation&) = delete;
    CopyToClipboardOperation& operator = (
                                      const CopyToClipboardOperation&) = delete;
    virtual ~CopyToClipboardOperation() noexcept = default;

    inline const std::string& mimeType() const noexcept;

    virtual bool objectSupported(const Object& object) const noexcept = 0;
    virtual void copyToClipboard(const std::vector<Object*>& objects);

  protected:
    /// objectSaver and objectFactory must not be nullptr
    void addSaver(std::string categoryName,
                  std::unique_ptr<ObjectSaver> objectSaver,
                  std::unique_ptr<ObjectFactory> objectFactory);

  private:
    std::set<Object*> _clearObjects(const std::vector<Object*>& objects);

  private:
    struct SaverRecord
    {
      std::string categoryName;
      std::unique_ptr<ObjectSaver> saver;
      std::unique_ptr<ObjectFactory> factory;

      SaverRecord() = default;
      SaverRecord(const SaverRecord&) = delete;
      SaverRecord(SaverRecord&&) = default;
      SaverRecord& operator = (const SaverRecord&) = delete;
      SaverRecord& operator = (SaverRecord&&) = default;
      ~SaverRecord() noexcept = default;
    };

    std::vector<SaverRecord> _savers;
    std::string _mimeType;
  };

  inline const std::string& CopyToClipboardOperation::mimeType() const noexcept
  {
    return _mimeType;
  }
}