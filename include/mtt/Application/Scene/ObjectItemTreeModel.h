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
  class UndoStack;

  class ObjectItemTreeModel : public QAbstractItemModel
  {
    Q_OBJECT

  public:
    /// undoStack is used to store drag and drop operations.
    /// You can set nullptr undoStack.
    ObjectItemTreeModel(Object& rootObject, UndoStack* undoStack);
    ObjectItemTreeModel(const ObjectItemTreeModel&) = delete;
    ObjectItemTreeModel& operator = (const ObjectItemTreeModel&) = delete;
    virtual ~ObjectItemTreeModel() noexcept;
  
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

    virtual Qt::DropActions supportedDropActions() const override;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QStringList mimeTypes() const override;
    virtual QMimeData* mimeData(const QModelIndexList& indexes) const override;
    virtual bool dropMimeData(const QMimeData* data,
                              Qt::DropAction action,
                              int row,
                              int column,
                              const QModelIndex& parent) override;  

    inline Object& rootObject() const noexcept;
  
    Object& getObject(const QModelIndex& index) const noexcept;
    QModelIndex getIndex(const Object& object) const noexcept;

  protected:
    struct ObjectDescription
    {
      inline ObjectDescription( Object& theObject,
                                ObjectDescription* theParent,
                                ObjectItemTreeModel& model) noexcept;
      ObjectDescription(const ObjectDescription&) = default;
      ObjectDescription& operator = (const ObjectDescription&) = default;
      inline ~ObjectDescription() noexcept;

      Object* object;
      ObjectDescription* parent;
      std::vector<QMetaObject::Connection> connections;
      std::vector<std::unique_ptr<ObjectDescription>> childs;

    private:
      ObjectItemTreeModel& _model;
    };

  protected:
    ObjectDescription& getDescription(const QModelIndex& index) const noexcept;
    QModelIndex buildIndex(ObjectDescription& description) const noexcept;

    virtual QVariant data(const Object& object, int role) const;
    virtual void connectToObject(ObjectDescription& description);

  private:
    std::unique_ptr<ObjectDescription> _buildDescription(
                                                    Object& object,
                                                    ObjectDescription* parent);
    void _updateName(ObjectDescription& description) noexcept;
    void _onChildAdded( ObjectDescription& groupDescription,
                        Object& child) noexcept;
    void _onChildRemoved( ObjectDescription& groupDescription,
                          Object& child) noexcept;

    void _filterMovedObjects( std::set<Object*>& objects,
                              const Object& targetGroup) const;

  private:
    Object& _rootObject;
    using DescriptionsMap = std::map<UID, ObjectDescription*>;
    DescriptionsMap _descriptionsMap;

    std::unique_ptr<ObjectDescription> _rootDescription;

    UndoStack* _undoStack;
  };

  inline Object& ObjectItemTreeModel::rootObject() const noexcept
  {
    return _rootObject;
  }

  inline ObjectItemTreeModel::ObjectDescription::ObjectDescription(
                                      Object& theObject,
                                      ObjectDescription* theParent,
                                      ObjectItemTreeModel& model) noexcept:
    object(&theObject),
    parent(theParent),
    _model(model)
  {
    _model._descriptionsMap[object->id()] = this;
  }

  inline ObjectItemTreeModel::ObjectDescription::~ObjectDescription() noexcept
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
      Abort("ObjectItemTreeModel::ObjectDescription::~ObjectDescription: unable to destruct the description.");
    }
  }
}