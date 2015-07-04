#include "Events.h"
//========================================================================
BaseEvent::BaseEvent(float timeStamp) :
m_timeStamp(timeStamp)
{
}
// -----------------------------------------------------------------------
void BaseEvent::VSetTimeStamp(float timestamp)
{
	m_timeStamp = timestamp;
}
//========================================================================
//Unique Event IDs
const EventType LookEvent::s_eventType = 0xfa52d07b;
