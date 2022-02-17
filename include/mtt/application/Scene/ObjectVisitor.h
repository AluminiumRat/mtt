#pragma once

namespace mtt
{
  class ObjectGroup;
  class Object;

  class ObjectVisitor
  {
  public:
    using ExtensionID = int;

  public:
    ObjectVisitor() = default;
    ObjectVisitor(const ObjectVisitor&) = delete;
    ObjectVisitor& operator = (const ObjectVisitor&) = delete;
    virtual ~ObjectVisitor() noexcept = default;

    virtual void process(const Object& object);
    virtual void process(Object& object);

    /// Returns unique id for extension
    static ExtensionID registerExtension() noexcept;

    template<typename ExtensionClass>
    inline ExtensionClass* getExtension() noexcept;

    virtual void visitConstObjectGroup(const ObjectGroup& object);
    virtual void visitObjectGroup(ObjectGroup& object);

    virtual void visitConstObject(const Object& object);
    virtual void visitObject(Object& object);

  protected:
    inline virtual void* getExtension(ExtensionID id) noexcept;
  };

  template<typename ExtensionClass>
  inline ExtensionClass* ObjectVisitor::getExtension() noexcept
  {
    void* extensionPtr = getExtension(ExtensionClass::extensionID);
    return static_cast<ExtensionClass*>(extensionPtr);
  }

  inline void* ObjectVisitor::getExtension(ExtensionID id) noexcept
  {
    return nullptr;
  }
}