#ifndef IEVENT_H
#define IEVENT_H

typedef unsigned long EventType;
class IEvent
{
public:
	virtual EventType VGetEventType() const = 0;
	virtual const float VGetTimeStamp() const = 0;
};
#endif