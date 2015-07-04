#pragma once
#include "../EventManager/Events/Events.h"
//========================================================================
class GraphicsLoadedEvent : public BaseEvent
{
public:
	GraphicsLoadedEvent();
	~GraphicsLoadedEvent();
	EventType VGetEventType() const override { return s_eventType; }
public:
	static const EventType s_eventType;
};
//========================================================================