#include <stdexcept>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>

#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/application/EditCommands/MoveObjectCommand.h>
#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/application/Scene/ObjectGroup.h>
#include <mtt/application/Scene/ObjectItemTreeModel.h>
#include <mtt/application/Scene/SpecialGroup.h>
#include <mtt/application/Application.h>

class ObjectWithValue : public mtt::Object
{
  Q_OBJECT

public:
  ObjectWithValue(const QString& name) :
    mtt::Object(name, false),
    _value(0)
  {
  }

  int value() const noexcept
  {
    return _value;
  }

  void setValue(int newValue)
  {
    if(_value == newValue) return;
    _value = newValue;
    mtt::Log() << "New value: " << newValue;
  }

private:
  int _value;
};

#include "main.moc"

void buildObjcts(mtt::ObjectGroup& root, mtt::UndoStack& undoStack)
{
  std::unique_ptr<mtt::ObjectGroup> group(new mtt::ObjectGroup("Group", false));
  mtt::ObjectGroup& groupRef = *group;

  undoStack.addAndMake(
                    std::make_unique<mtt::AddObjectCommand>(std::move(group),
                                                            root));

  std::unique_ptr<ObjectWithValue> child(new ObjectWithValue("Child"));
  ObjectWithValue& childRef = *child;

  undoStack.addAndMake(
                    std::make_unique<mtt::AddObjectCommand>(std::move(child),
                                                            root));

  undoStack.addAndMake( std::make_unique<mtt::MoveObjectCommand>( childRef,
                                                                  groupRef));

  using SetValueCommand = mtt::SetPropertyCommand<
                                      ObjectWithValue,
                                      int,
                                      int(ObjectWithValue::*)() const noexcept,
                                      void(ObjectWithValue::*)(int)>;
  std::unique_ptr<SetValueCommand> setCommand(
                                new SetValueCommand(childRef,
                                                    &ObjectWithValue::value,
                                                    &ObjectWithValue::setValue,
                                                    10));
  undoStack.addAndMake(std::move(setCommand));
}

void fillWindow(QWidget& window,
                mtt::ObjectItemTreeModel& objectModel,
                mtt::UndoStack& undoStack)
{
  QVBoxLayout* layout = new QVBoxLayout(&window);

  QTreeView* treeWidget(new QTreeView);
  layout->addWidget(treeWidget);
  treeWidget->setModel(&objectModel);

  QPushButton* undoButton = new QPushButton("Undo");
  layout->addWidget(undoButton);
  undoButton->connect(undoButton,
                      &QPushButton::clicked,
                      [&]()
                      {
                        undoStack.undoCommand();
                      });

  QPushButton* redoButton = new QPushButton("Redo");
  layout->addWidget(redoButton);
  redoButton->connect(redoButton,
                      &QPushButton::clicked,
                      [&]()
                      {
                        undoStack.redoCommand();
                      });
}

int main(int argc, char* argv[])
{
  try
  {
    VkPhysicalDeviceFeatures features{};
    mtt::Application application( argc,
                                  argv,
                                  "Mtt commands example",
                                  { 0, 0, 0 },
                                  VK_API_VERSION_1_2,
                                  features,
                                  false);

    mtt::UndoStack undoStack(0);
    mtt::ObjectGroup root("Root", false);
    buildObjcts(root, undoStack);

    mtt::ObjectItemTreeModel objectModel(root, nullptr);
    QWidget window;
    fillWindow(window, objectModel, undoStack);
    window.show();

    return application.exec();
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    return 1;
  }
}
