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

  protected:
    inline virtual void* getExtension(ExtensionID id) noexcept;

  public:
    virtual void visit(const ObjectGroup& object);
    virtual void visit(ObjectGroup& object);

    virtual void visit(const Object& object);
    virtual void visit(Object& object);
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