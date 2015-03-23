#ifndef IEVENTLISTENER_H
#define IEVENTLISTENER_H
#include "..\AlphaStd.h"
#include "IEvent.h"
class IEventListener
{
public:
	//called by EventManager
	virtual bool VOnEvent(shared_ptr<IEvent> event) = 0;
	//used to register for appropriate event types
	virtual bool VRegisterListener() = 0;
	//used to enforce deregistration of listener
	virtual bool VDeregisterListener() = 0;
};
#endif