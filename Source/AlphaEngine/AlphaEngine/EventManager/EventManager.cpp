#include "EventManager.h"
#include "../Common/GameContext.h"
EventManager::EventManager(const char* name, bool setAsGlobal) : 
IEventManager(setAsGlobal),
m_name(name),
m_isGlobal(setAsGlobal)
{
}
// -----------------------------------------------------------------------
EventManager::~EventManager()
{
	//release all
	m_eventListeners.clear();
	m_eventQueue.clear();
}
// -----------------------------------------------------------------------
const char* EventManager::GetName() const
{
	return m_name;
}
// -----------------------------------------------------------------------
bool EventManager::VRegisterListener(EventType eventType, ListenerDelegate listener)
{
	EventListenerList* listenerList = &m_eventListeners[eventType];
	//check if the listener is already registered for this type of event
	for (auto it = (*listenerList).begin(); it != (*listenerList).end(); it++)
	{
		if (listener == (*it))
		{
			//already registered
			return false;
		}
	}
	//add listener to appropriate list
	m_eventListeners[eventType].push_back(listener);
	return true;
}
// -----------------------------------------------------------------------
bool EventManager::VDeregisterListener(EventType eventType, ListenerDelegate listener)
{
	EventListenerList* listenerList = &m_eventListeners[eventType];

	//check if the listener is registered for this type of event
	for (auto it = (*listenerList).begin(); it != (*listenerList).end(); it++)
	{
		if (listener == (*it))
		{
			//remove listener from this list
			m_eventListeners[eventType].erase(it);
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------
bool EventManager::VUpdate()
{
	while (!m_eventQueue.empty())
	{
		//get first event in queue and remove it
		shared_ptr<IEvent> event = m_eventQueue.front();
		m_eventQueue.pop_front();
		//get list mapped to that event type
		EventType type = event->VGetEventType();
		EventListenerList* listenerList = &m_eventListeners[type];
		//call the "VOnEvent" for all listeners  in that list
		for (auto it = (*listenerList).begin(); it != (*listenerList).end(); it++)
		{
			//invoke listener delegate
			(*it)(event);
		}
	}
	return true;
}
// -----------------------------------------------------------------------
bool EventManager::VQueueEvent(shared_ptr<IEvent> event)
{
	if (!event)
	{
		return false;
	}
	event->VSetTimeStamp(GameContext::Get()->GetGameTime());
	m_eventQueue.push_back(event);
	return true;
}
// -----------------------------------------------------------------------
bool EventManager::VAbortEvent(shared_ptr<IEvent> event)
{
	//remove event from queue if found
	for (auto it = m_eventQueue.begin(); it != m_eventQueue.end(); it++)
	{
		if ((*it) == event)
			m_eventQueue.erase(it);
		return true;
	}
	
	return false;
}
// -----------------------------------------------------------------------