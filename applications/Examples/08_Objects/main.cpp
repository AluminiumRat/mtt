#include <stdexcept>

#include <QtWidgets/QTreeView>

#include <mtt/application/Scene/Object.h>
#include <mtt/application/Scene/ObjectGroup.h>
#include <mtt/application/Scene/ObjectItemTreeModel.h>
#include <mtt/application/Scene/SpecialGroup.h>
#include <mtt/application/Application.h>

class Book : public mtt::Object
{
  Q_OBJECT

public:
  Book(const QString& name) :
    Object(name, false)
  {
  }
};

class Human : public mtt::Object
{
  Q_OBJECT

public:
  Human(const QString& name) :
    Object(name, false)
  {
  }
};

class Bag : public mtt::SpecialGroup<mtt::Object, Book>
{
  Q_OBJECT

public:
  Bag(const QString& name) :
    SpecialGroup(name, false)
  {
  }

signals:
  void childAdded(Book& object) override;
  void childRemoved(Book& object) override;
};

class Kiosk : public mtt::Object
{
  Q_OBJECT

public:
  Kiosk(const QString& name) :
    Object(name, false)
  {
    addSubobject(std::make_unique<Human>("Cook"));
    addSubobject(std::make_unique<Book>("Menu"));
  }
};

class RootObject : public mtt::Object
{
  Q_OBJECT

public:
  RootObject() :
    Object("Root", false)
  {
    std::unique_ptr<Bag> bag(new Bag("Bag"));
    bag->addChild(std::make_unique<Book>("Book1"));
    bag->addChild(std::make_unique<Book>("Book2"));
    addSubobject(std::move(bag));

    addSubobject(std::make_unique<Kiosk>("Kiosk"));

    std::unique_ptr<mtt::ObjectGroup> room(
                                          new mtt::ObjectGroup("Room", false));
    room->addChild(std::make_unique<Book>("Book3"));
    room->addChild(std::make_unique<Human>("Human"));
    addSubobject(std::move(room));
  }
};

#include "main.moc"

int main(int argc, char* argv[])
{
  try
  {
    VkPhysicalDeviceFeatures features{};
    mtt::Application application( argc,
                                  argv,
                                  "Mtt objects example",
                                  { 0, 0, 0 },
                                  VK_API_VERSION_1_2,
                                  features,
                                  false);

    RootObject root;

    mtt::ObjectItemTreeModel objectModel(root, nullptr);

    QTreeView treeWidget;
    treeWidget.setModel(&objectModel);
    treeWidget.setDragDropMode(QAbstractItemView::InternalMove);
    treeWidget.show();

    return application.exec();
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    return 1;
  }
}
