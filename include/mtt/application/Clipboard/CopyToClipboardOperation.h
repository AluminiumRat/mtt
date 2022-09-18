#pragma once

#include <memory>
#include <set>
#include <vector>

#include <QtCore/QString>

#include <mtt/application/Scene/ObjectSaver.h>

namespace mtt
{
  class CopyToClipboardOperation
  {
  public:
    /// objectSaver and objectFactory must not be nullptr
    CopyToClipboardOperation( const QString& mimeType,
                              std::unique_ptr<ObjectSaver> objectSaver,
                              std::unique_ptr<ObjectFactory> objectFactory);
    CopyToClipboardOperation(const CopyToClipboardOperation&) = delete;
    CopyToClipboardOperation& operator = (
                                      const CopyToClipboardOperation&) = delete;
    virtual ~CopyToClipboardOperation() noexcept = default;

    virtual void copyToClipboard(const std::vector<Object*>& objects);

  private:
    std::set<Object*> _clearObjects(const std::vector<Object*>& objects);

  private:
    QString _mimeType;
    std::unique_ptr<ObjectSaver> _objectSaver;
    std::unique_ptr<ObjectFactory> _objectFactory;
  };
}