#pragma once

#include <deque>
#include <memory>

#include <QtCore/QObject>

#include <mtt/application/EditCommands/AbstractEditCommand.h>

namespace mtt
{
  class UndoStack : public QObject,
                    protected AbstractEditCommand
  {
    Q_OBJECT

  public:
    class GroupLocker
    {
    public:
      inline GroupLocker();
      inline explicit GroupLocker(UndoStack& stack);
      GroupLocker(const GroupLocker&) = delete;
      inline GroupLocker(GroupLocker&& other) noexcept;
      GroupLocker& operator = (const GroupLocker&) = delete;
      inline GroupLocker& operator = (GroupLocker&& other) noexcept;
      inline ~GroupLocker() noexcept;

      inline void reset(UndoStack& stack);
      inline void reset() noexcept;

    private:
      UndoStack* _stack;
    };

  public:
    /// Use 0 to make unlimited stack
    explicit UndoStack(size_t stackSize);
    UndoStack(const UndoStack&) = delete;
    UndoStack& operator = (const UndoStack&) = delete;
    virtual ~UndoStack() noexcept = default;

    virtual void addAndMake(std::unique_ptr<AbstractEditCommand> command);
    virtual void addCommand(std::unique_ptr<AbstractEditCommand> command);

    inline bool undoAvailable() const noexcept;
    inline bool redoAvailable() const noexcept;

  public slots:
    virtual void undoCommand();
    virtual void redoCommand();
    virtual void clear() noexcept;

  signals:
    void undoAvailableChanged(bool available);
    void redoAvailableChanged(bool available);

  protected:
    virtual void make() override;
    virtual void undo() override;
  
    virtual void lockGroup();
    virtual void ulockGroup() noexcept;

  private:
    void _dropTail();
    void _updateAvailables() noexcept;

  private:
    size_t _stackSize;
    using Commands = std::deque<std::unique_ptr<AbstractEditCommand>>;
    Commands _commands;
    Commands::iterator _cursor;
    bool _undoAvailable;
    bool _redoAvailable;
  
    std::unique_ptr<UndoStack> _substack;
    size_t _substackUseCounter;
  };

  inline bool UndoStack::undoAvailable() const noexcept
  {
    return _undoAvailable;
  }

  inline bool UndoStack::redoAvailable() const noexcept
  {
    return _redoAvailable;
  }

  inline UndoStack::GroupLocker::GroupLocker() :
    _stack(nullptr)
  {
  }

  inline UndoStack::GroupLocker::GroupLocker(UndoStack& stack) :
    _stack(nullptr)
  {
    reset(stack);
  }

  inline UndoStack::GroupLocker::GroupLocker(GroupLocker&& other) noexcept :
    _stack(other._stack)
  {
    other._stack = nullptr;
  }

  inline UndoStack::GroupLocker& 
                UndoStack::GroupLocker::GroupLocker::operator = (
                                                  GroupLocker&& other) noexcept
  {
    reset();
    _stack = other._stack;
    other._stack = nullptr;
    return *this;
  }

  inline UndoStack::GroupLocker::~GroupLocker() noexcept
  {
    reset();
  }

  inline void UndoStack::GroupLocker::reset(UndoStack& stack)
  {
    reset();
    stack.lockGroup();
    _stack = &stack;
  }

  inline void UndoStack::GroupLocker::reset() noexcept
  {
    if (_stack != nullptr)
    {
      _stack->ulockGroup();
      _stack = nullptr;
    }
  }
}