#ifndef EVENTS_H
#define EVENTS_H
#include "..\IEvent.h"

//base event from which all events inherit
class BaseEvent : public IEvent
{

public:
	explicit BaseEvent(const float timeStamp = 0.0f) :
	m_timeStamp(timeStamp)
	{		
	}
	virtual EventType VGetEventType() const override
	{
		return sEventType;
	}

	virtual const float VGetTimeStamp() const override
	{
		return m_timeStamp;
	}
public:
	static const EventType sEventType;
private:
	const float m_timeStamp;
};
#endif