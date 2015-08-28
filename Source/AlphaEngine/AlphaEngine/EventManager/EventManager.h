#pragma once
#include "IEventManager.h"
//========================================================================
class DLLExport EventManager : public IEventManager
{
public:
	EventManager(const char* name, bool setAsGlobal);
	~EventManager();
	const char* GetName() const;
	//register listener
	virtual bool VRegisterListener(EventType eventType, ListenerDelegate listener) override;
	//deregister listener
	virtual bool VDeregisterListener(EventType eventType, ListenerDelegate listener) override;
	//called once per frame
	virtual bool VUpdate() override;
	//add event to queue
	virtual bool VQueueEvent(StrongEventPtr event) override;
	//remove event from queue
	virtual bool VAbortEvent(StrongEventPtr event) override;
private:
	const char* m_name;
	bool m_isGlobal;
	EventListenerMap m_eventListeners;
	EventQueue m_eventQueue;
};
//========================================================================
