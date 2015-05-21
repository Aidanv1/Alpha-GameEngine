#pragma once

typedef unsigned long EventType;
class IEvent
{
public:
	virtual EventType VGetEventType() const = 0;
	virtual const float VGetTimeStamp() const = 0;
};
