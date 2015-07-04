#pragma once

typedef unsigned long EventType;
class IEvent
{
	friend class EventManager;
public:
	virtual EventType VGetEventType() const = 0;
	virtual const float VGetTimeStamp() const = 0;
private:
	virtual void VSetTimeStamp(float timestamp) = 0;
};
