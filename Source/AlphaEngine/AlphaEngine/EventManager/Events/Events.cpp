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
LookEvent::LookEvent(const float timeStamp) :
BaseEvent(timeStamp)
{
}
// -----------------------------------------------------------------------
void LookEvent::SetLook(float dThetaX, float dThetaY, float dZ, float dX, float dY)
{
	m_lookPos.m_dThetaX = dThetaX;
	m_lookPos.m_dThetaY = dThetaY;
	m_lookPos.m_dZ = dZ;
	m_lookPos.m_dX = dX;
	m_lookPos.m_dY = dY;
}
// -----------------------------------------------------------------------
LookEvent::LookPos LookEvent::GetLookPos() const
{ 
	return m_lookPos; 
}
// -----------------------------------------------------------------------
EventType LookEvent::VGetEventType() const
{
	return s_eventType;
}

//========================================================================
//Unique Event IDs
const EventType LookEvent::s_eventType = 0xfa52d07b;
