#pragma once

#include <vector>

#include <QtCore/QObject>

#include <mtt/Application/Scene/ObjectVisitor.h>
#include <mtt/Utilities/UID.h>

#define DEFINE_VISITOR_ACCEPT                                               \
protected:                                                                  \
  inline virtual void accept(ObjectVisitor& visitor) override               \
  {                                                                         \
    visitor.visit(*this);                                                   \
  }                                                                         \
  inline virtual void accept(ObjectVisitor& visitor) const override         \
  {                                                                         \
    visitor.visit(*this);                                                   \
  }

#define DEFINE_EXTENSION_ACCEPT(ExtensionClass)                             \
protected:                                                                  \
  inline virtual void accept(mtt::ObjectVisitor& visitor) override          \
  {                                                                         \
    ExtensionClass* extension = visitor.getExtension<ExtensionClass>();     \
    if(extension != nullptr) extension->visit(*this);                       \
    else visitor.visit(*this);                                              \
  }                                                                         \
  inline virtual void accept(mtt::ObjectVisitor& visitor) const override    \
  {                                                                         \
    ExtensionClass* extension = visitor.getExtension<ExtensionClass>();     \
    if(extension != nullptr) extension->visit(*this);                       \
    else visitor.visit(*this);                                              \
  }

namespace mtt
{
  class ObjectRefBase;
  class Scene;

  class Object : public QObject
  {
    Q_OBJECT

    Q_PROPERTY( UID id
                READ id
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

  Q_PROPERTY( QString name
              READ name
              WRITE tryRename
              NOTIFY nameChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  protected:
    friend class ObjectVisitor;
    virtual void accept(ObjectVisitor& visitor);
    virtual void accept(ObjectVisitor& visitor) const;

  public:
    Object( const QString& name, bool canBeRenamed, const UID& id = UID());
    Object(const Object&) = delete;
    Object& operator = (const Object&) = delete;
    virtual ~Object() noexcept;
  
    inline const UID& id() const noexcept;

    inline QString name() const noexcept;
    inline bool canBeRenamed() const noexcept;
    /// If canBeRenamed property is true, the name will be changed to newVale
    /// Otherwise nothing will happen
    virtual void tryRename(const QString& newVale);

    inline Scene* scene() const noexcept;

    inline Object* parent() const noexcept;

    inline size_t subobjectNumber() const noexcept;
    inline Object& subobject(size_t subobjectIndex) noexcept;
    inline const Object& subobject(size_t subobjectIndex) const noexcept;

    /// This is only type check. No entry check.
    virtual bool subobjectCanBeAdded(const Object& subobject) const noexcept;
    /// Returns pointer to the subobject if addition is not possible.
    /// Returns nullptr if addition was success.
    virtual std::unique_ptr<Object> tryAddSubobject(
                                                std::unique_ptr<Object> object);

    /// This is only type check. No entry check.
    virtual bool subobjectCanBeRemoved(const Object& subobject) const noexcept;
    /// Returns removed object if success. Returns nullptr otherwise.
    virtual std::unique_ptr<Object> tryRemoveSubobject(
                                                Object& subobject,
                                                bool removeFromScene) noexcept;

    /// This is only type check. No entry check.
    inline bool subobjectCanBeAddedAndRemoved(
                                        const Object& subobject) const noexcept;

    virtual void notifyAboutAppearance(Object& object);
    virtual void notifyAboutDisappearance(Object& object) noexcept;

  signals:
    void nameChanged(const QString& newName);
    void subobjectAdded(Object& object);
    void subobjectRemoved(Object& object);

  protected:
    void addSubobject(std::unique_ptr<Object> object);
    /// Called from addSubobject after insertion and before signal emitting
    virtual void onSubobjectAdded(Object& object) noexcept;

    /// Returns removed object if success. Returns nullptr otherwise.
    std::unique_ptr<Object> removeSubobject(Object& object,
                                            bool removeFromScene) noexcept;
    /// Called from removeSubobject after removing and before signal emitting
    virtual void onSubobjectRemoved(Object& object) noexcept;

    friend class ObjectRefBase;
    virtual void addLink(ObjectRefBase& link);
    virtual void removeLink(ObjectRefBase& link) noexcept;
    virtual void updateLink(ObjectRefBase& link);

  private:
    void _setParent(Object* newParent) noexcept;
    friend class Scene;
    void _setScene(Scene* newScene);

  private:
    UID _id;
    QString _name;
    bool _canBeRenamed;
    Scene* _scene;
    Object* _parent;
    using Subobjects = std::vector<Object*>;
    Subobjects _subobjects;
  
    using Links = std::vector<ObjectRefBase*>;
    Links _links;
  };

  inline const UID& Object::id() const noexcept
  {
    return _id;
  }

  inline QString Object::name() const noexcept
  {
    return _name;
  }

  inline bool Object::canBeRenamed() const noexcept
  {
    return _canBeRenamed;
  }

  inline Scene* Object::scene() const noexcept
  {
    return _scene;
  }

  inline Object* Object::parent() const noexcept
  {
    return _parent;
  }

  inline size_t Object::subobjectNumber() const noexcept
  {
    return _subobjects.size();
  }

  inline Object& Object::subobject(size_t subobjectIndex) noexcept
  {
    return *_subobjects[subobjectIndex];
  }

  inline const Object& Object::subobject(size_t subobjectIndex) const noexcept
  {
    return *_subobjects[subobjectIndex];
  }

  inline bool Object::subobjectCanBeAddedAndRemoved(
                                        const Object& subobject) const noexcept
  {
    return subobjectCanBeAdded(subobject) && subobjectCanBeRemoved(subobject);
  }
}