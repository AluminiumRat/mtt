#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include <QtCore/QAbstractItemModel>

#include <mtt/Application/Scene/Object.h>
#include <mtt/Utilities/Abort.h>
#include <mtt/Utilities/UID.h>

namespace mtt
{
  class Object;

  class ObjectItemListModel : public QAbstractItemModel
  {
    Q_OBJECT

  public:
    ObjectItemListModel(Object& rootObject);
    ObjectItemListModel(const ObjectItemListModel&) = delete;
    ObjectItemListModel& operator = (const ObjectItemListModel&) = delete;
    virtual ~ObjectItemListModel() noexcept = default;
  
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role) const override;

    virtual QModelIndex index(int row,
                              int column,
                              const QModelIndex& parent) const override;
    virtual QModelIndex parent(const QModelIndex& index) const override;
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;

    inline Object& rootObject() const noexcept;
  
    Object& getObject(const QModelIndex& index) const noexcept;
    QModelIndex getIndex(const Object& object) const noexcept;

  protected:
    struct ObjectDescription
    {
      inline ObjectDescription( Object& theObject,
                                int theIndex,
                                ObjectItemListModel& model) noexcept;
      ObjectDescription(const ObjectDescription&) = default;
      ObjectDescription& operator = (const ObjectDescription&) = default;
      inline ~ObjectDescription() noexcept;

      Object* object;
      int index;
      std::vector<QMetaObject::Connection> connections;

    private:
      ObjectItemListModel& _model;
    };

  protected:
    ObjectDescription& getDescription(const QModelIndex& index) const noexcept;
    QModelIndex buildIndex(ObjectDescription& description) const noexcept;

    virtual QVariant data(const Object& object, int role) const;
    virtual void connectToObject(ObjectDescription& description);

  private:
    void _updateName(ObjectDescription& description) noexcept;
    void _addItems(Object& object) noexcept;
    void _removeItems(Object& object) noexcept;

  private:
    Object& _rootObject;
    using DescriptionsMap = std::map<UID, ObjectDescription*>;
    DescriptionsMap _descriptionsMap;

    using Descriptions = std::vector<std::unique_ptr<ObjectDescription>>;
    Descriptions _descriptions;
  };

  inline Object& ObjectItemListModel::rootObject() const noexcept
  {
    return _rootObject;
  }

  inline ObjectItemListModel::ObjectDescription::ObjectDescription(
                                          Object& theObject,
                                          int theIndex,
                                          ObjectItemListModel& model) noexcept:
    object(&theObject),
    index(theIndex),
    _model(model)
  {
    _model._descriptionsMap[object->id()] = this;
  }

  inline ObjectItemListModel::ObjectDescription::~ObjectDescription() noexcept
  {
    try
    {
      for(QMetaObject::Connection connection : connections)
      {
        QObject::disconnect(connection);
      }
      _model._descriptionsMap.erase(object->id());
    }
    catch(...)
    {
      Abort("ObjectItemListModel::ObjectDescription::~ObjectDescription: unable to destruct the description.");
    }
  }
}