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


using namespace std;




class TestClass: public IEventListener
{
	public:
	TestClass()
	{
		VRegisterListener();
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
		return true;
	}
};


int main(void)
{

	EventManager EM("test", true);
	TestClass test;
	IEvent* be = new BaseEvent();
	shared_ptr<IEvent> pbe(new BaseEvent());
	IEventManager::Get()->VQueueEvent(pbe);
	IEventManager::Get()->VQueueEvent(pbe);
	IEventManager::Get()->VQueueEvent(pbe);

	IEventManager::Get()->VUpdate();
	system("pause");

}
	
