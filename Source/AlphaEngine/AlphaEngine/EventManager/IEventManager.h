#pragma once
#include "IEvent.h"
#include "..\AlphaStd.h"
#include "FastDelegate.h"

typedef shared_ptr<IEvent> StrongEventPtr;
typedef fastdelegate::FastDelegate1<StrongEventPtr> ListenerDelegate;
//========================================================================
class IEventManager
{
	static IEventManager* globalManager;

protected:
	typedef vector<ListenerDelegate> EventListenerList;
	typedef map<EventType, EventListenerList> EventListenerMap;
	typedef list<shared_ptr<IEvent>> EventQueue;

public:
	explicit IEventManager(bool setAsGlobal);
	virtual ~IEventManager();
	//register listener
	virtual bool VRegisterListener(EventType eventType, ListenerDelegate listener) = 0;
	//deregister listener
	virtual bool VDeregisterListener(EventType eventType, ListenerDelegate listener) = 0;
	//called once per frame
	virtual bool VUpdate() = 0;
	//add event to queue
	virtual bool VQueueEvent(shared_ptr<IEvent> event) = 0;
	//remove event from queue
	virtual bool VAbortEvent(shared_ptr<IEvent> event) = 0;
	//get singleton
	static IEventManager* Get();
};
//========================================================================
//								MACROS
//These are some macros that can be used to easily interact with the
//global event manager.
// -----------------------------------------------------------------------
template <class X, class Y, class Param1, class RetType>
inline void Register_Listener(EventType eventType, Y* x, RetType(X::*func)(Param1 p1))
{
	ListenerDelegate delegateListener = fastdelegate::MakeDelegate(x, func);
	IEventManager::Get()->VRegisterListener(eventType, delegateListener);
}
template <class X, class Y, class Param1, class RetType>
inline void Register_Listener(Y* x, RetType(X::*func)(Param1 p1) const)
{
	ListenerDelegate delegateListener = fastdelegate::MakeDelegate(x, func(p1));
	IEventManager::Get()->VRegisterListener(eventType, delegateListener);
}
// -----------------------------------------------------------------------
inline void Queue_Event(IEvent* event)
{
	IEventManager::Get()->VQueueEvent(StrongEventPtr(event));
}
// -----------------------------------------------------------------------
inline void Abort_Event(IEvent* event)
{
	IEventManager::Get()->VAbortEvent(StrongEventPtr(event));
}
//========================================================================
