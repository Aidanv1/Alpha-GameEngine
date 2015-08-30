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

