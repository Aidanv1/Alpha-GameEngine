#include <string>
#include <iostream>
#include "Time\Clock.h"
#include "Time\ClockManager.h"
#include "Time\SystemTime.h"

#include "Memory\MemoryPool.h"
#include "ResourceManager\ResourceManager.h"

#include "EventManager\Events\Events.h"
#include "EventManager\IEvent.h"
#include "EventManager\EventManager.h"

#include "GameObject\GameObjectFactory.h"
#include "GameObject\TestComponentCreator.h"
using namespace std;




class TestClass: public IEventListener
{
	public:
	TestClass()
	{
		VRegisterListener();
	}
	~TestClass()
	{
		VDeregisterListener();
	}
	virtual bool VOnEvent(shared_ptr<IEvent> event)
	{
		cout << "hello";
		return true;
	}
	//used to register for appropriate event types
	virtual bool VRegisterListener()
	{
		IEventManager::Get()->VRegisterListener(BaseEvent::sEventType, this);
		return true;
	}
	//used to enforce deregistration of listener
	virtual bool VDeregisterListener()
	{
		IEventManager::Get()->VDeregisterListener(BaseEvent::sEventType, this);
		return true;
	}
};


int main(void)
{

	//EventManager EM("test", true);
	//TestClass* test = new TestClass();

	//IEvent* be = new BaseEvent();
	//shared_ptr<IEvent> pbe(new BaseEvent());
	//IEventManager::Get()->VQueueEvent(pbe);
	//IEventManager::Get()->VQueueEvent(pbe);
	//IEventManager::Get()->VQueueEvent(pbe);
	//delete test;
	//IEventManager::Get()->VUpdate();
	//cout << endl;
	//IEventManager::Get()->VQueueEvent(pbe);
	//IEventManager::Get()->VUpdate();

	GameObjectFactory gf;
	IComponentCreator* tc = new TestComponentCreator();
	gf.AddComponentCreator(tc, "test");
	TiXmlDocument doc("test.xml");
	bool loadOkay = doc.LoadFile();
	TiXmlElement* ele = doc.RootElement()->FirstChildElement();

	shared_ptr<GameObject> go = gf.CreateGameObject(ele);
	TestComponent* t = (TestComponent*)go->GetComponent(1);
	cout << t->GetName()<< endl;
	cout << t->GetOwnerId() << endl;

	go->Destroy();
	system("pause");

}
	
