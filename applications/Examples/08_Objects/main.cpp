#include <stdexcept>

#include <QtWidgets/QTreeView>

#include <mtt/application/Scene/Object.h>
#include <mtt/application/Scene/ObjectGroup.h>
#include <mtt/application/Scene/ObjectItemTreeModel.h>
#include <mtt/application/Scene/SpecialGroup.h>
#include <mtt/application/Application.h>

class ChildA : public mtt::Object
{
  Q_OBJECT

public:
  ChildA(const QString& name) :
    Object(name, false)
  {
  }
};

class GroupA : public mtt::SpecialGroup<mtt::Object, ChildA>
{
  Q_OBJECT

public:
  GroupA(const QString& name) :
    SpecialGroup(name, false)
  {
  }

signals:
  void childAdded(ChildA& object) override;
  void childRemoved(ChildA& object) override;
};

class ChildB : public mtt::Object
{
  Q_OBJECT

public:
  ChildB(const QString& name) :
    Object(name, false)
  {
  }
};

class ParentB : public mtt::Object
{
  Q_OBJECT

public:
  ParentB(const QString& name) :
    Object(name, false)
  {
    addSubobject(std::make_unique<ChildA>("SubobjectA"));
    addSubobject(std::make_unique<ChildB>("SubobjectB"));
  }
};

class RootObject : public mtt::Object
{
  Q_OBJECT

public:
  RootObject() :
    Object("Root", false)
  {
    std::unique_ptr<GroupA> groupA(new GroupA("GroupA"));
    groupA->addChild(std::make_unique<ChildA>("ChildA1"));
    groupA->addChild(std::make_unique<ChildA>("ChildA2"));
    addSubobject(std::move(groupA));

    addSubobject(std::make_unique<ParentB>("GroupB"));

    std::unique_ptr<mtt::ObjectGroup> freeGroup(
                                      new mtt::ObjectGroup("FreeGroup", false));
    freeGroup->addChild(std::make_unique<ChildA>("ChildA3"));
    freeGroup->addChild(std::make_unique<ChildB>("ChildB"));
    addSubobject(std::move(freeGroup));
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
    treeWidget.setDragDropMode(QAbstractItemView::InternalMove);
    treeWidget.setModel(&objectModel);
    treeWidget.show();

    return application.exec();
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    return 1;
  }
}
