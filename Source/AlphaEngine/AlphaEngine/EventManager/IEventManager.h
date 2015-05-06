#ifndef IEVENTMANAGER_H
#define IEVENTMANAGER_H
#include "IEventListener.h"
#include "..\AlphaStd.h"
class IEventManager
{
	static IEventManager* globalManager;

protected:
	typedef vector<IEventListener*> EventListenerList;
	typedef map<EventType, EventListenerList> EventListenerMap;
	typedef list<shared_ptr<IEvent>> EventQueue;

public:
	explicit IEventManager(bool setAsGlobal);
	virtual ~IEventManager();
	//register listener
	virtual bool VRegisterListener(EventType eventType, IEventListener* listener) = 0;
	//deregister listener
	virtual bool VDeregisterListener(EventType eventType, IEventListener* listener) = 0;
	//called once per frame
	virtual bool VUpdate() = 0;
	//add event to queue
	virtual bool VQueueEvent(shared_ptr<IEvent> event) = 0;
	//remove event from queue
	virtual bool VAbortEvent(shared_ptr<IEvent> event) = 0;
	//get singleton
	static IEventManager* Get();
};
#endif