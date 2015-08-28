#pragma once
#include "../AlphaEngineDLL.h"
#include "IEvent.h"
#include "FastDelegate/FastDelegate.h"

#include <memory>
#include <map>
#include <list>
#include <vector>
typedef std::shared_ptr<IEvent> StrongEventPtr;
typedef fastdelegate::FastDelegate1<StrongEventPtr> ListenerDelegate;
typedef std::vector<ListenerDelegate> EventListenerList;
typedef std::map<EventType, EventListenerList> EventListenerMap;
typedef std::list<StrongEventPtr> EventQueue;
//========================================================================
class DLLExport IEventManager
{
	static IEventManager* globalManager;
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
	virtual bool VQueueEvent(StrongEventPtr event) = 0;
	//remove event from queue
	virtual bool VAbortEvent(StrongEventPtr event) = 0;
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
template <class X, class Y, class Param1, class RetType>
inline void Deregister_Listener(EventType eventType, Y* x, RetType(X::*func)(Param1 p1))
{
	ListenerDelegate delegateListener = fastdelegate::MakeDelegate(x, func);
	IEventManager::Get()->VDeregisterListener(eventType, delegateListener);
}
template <class X, class Y, class Param1, class RetType>
inline void Deregister_Listener(Y* x, RetType(X::*func)(Param1 p1) const)
{
	ListenerDelegate delegateListener = fastdelegate::MakeDelegate(x, func(p1));
	IEventManager::Get()->VDeregisterListener(eventType, delegateListener);
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
