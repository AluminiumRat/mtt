#include <stdexcept>

#include <mtt/application/Scene/Object.h>
#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/application/Scene/Scene.h>
#include <mtt/application/Application.h>

class Bob : public mtt::Object
{
  Q_OBJECT

public:
  Bob() : Object("Bob", false)
  {
  }

  void toHammer()
  {
    mtt::Log() << "Bob hammers a bolt";
  }
};

class Bill : public mtt::Object
{
  Q_OBJECT

signals:
  void bobLinkChanged(Bob* bobPtr);

private:
  void onBobIncoming(Bob& bob)
  {
    mtt::Log() << "Hello, Bob!";
  }

  void onBobOutcoming(Bob& bob) noexcept
  {
    mtt::Log() << "Goodbye, Bob!";
  }

public:
  using BobLink = mtt::ObjectLink<Bob,
                                  Bill,
                                  &Bill::onBobIncoming,
                                  &Bill::onBobOutcoming,
                                  &Bill::bobLinkChanged>;
  BobLink bobLink;

public:
  Bill() :
    Object("Bill", false),
    bobLink(*this)
  {
  }

  void askBob()
  {
    if(bobLink != nullptr) bobLink->toHammer();
    else mtt::Log() << "Bob is gone";
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
                                  "Mtt object link example",
                                  { 0, 0, 0 },
                                  VK_API_VERSION_1_2,
                                  features,
                                  false);

    Bob bob;

    Bill bill;
    bill.bobLink.set(&bob);

    bill.askBob();

    mtt::Scene scene;
    scene.registerObject(bill);
    scene.registerObject(bob);

    bill.askBob();

    scene.unregisterObject(bob);

    bill.askBob();

    scene.registerObject(bob);

    bill.askBob();

    scene.unregisterObject(bill);

    bill.askBob();

    return 0;
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    return 1;
  }
}
