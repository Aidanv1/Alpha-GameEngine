#include "GraphicsEvents.h"
//========================================================================
GraphicsLoadedEvent::GraphicsLoadedEvent()
{
}
// -----------------------------------------------------------------------
GraphicsLoadedEvent::~GraphicsLoadedEvent()
{
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
	return EVENT_GRAPHICS_LOOKEVENT;
}

//========================================================================