#pragma once

#include <mtt/editorLib/Objects/EnvironmentObject.h>

namespace mtt
{
  class EnvironmentModel : public EnvironmentObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension)

    Q_PROPERTY( QString filename
                READ filename
                WRITE setFilename
                RESET resetFilename
                NOTIFY filenameChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)
  public:
    EnvironmentModel( const QString& name,
                      bool canBeRenmed,
                      const UID id = UID());
    EnvironmentModel(const EnvironmentModel&) = delete;
    EnvironmentModel& operator = (const EnvironmentModel&) = delete;
    virtual ~EnvironmentModel() noexcept = default;

    inline const QString& filename() const noexcept;
    virtual void setFilename(const QString& newValue) noexcept;
    inline void resetFilename() noexcept;

  signals:
    void filenameChanged(const QString&);

  private:
    QString _filename;
  };

  inline const QString& EnvironmentModel::filename() const noexcept
  {
    return _filename;
  }

  inline void EnvironmentModel::resetFilename() noexcept
  {
    setFilename(QString());
  }
}