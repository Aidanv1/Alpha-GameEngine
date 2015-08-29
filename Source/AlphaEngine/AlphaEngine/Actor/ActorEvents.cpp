#include "ActorEvents.h"
//========================================================================
//Event Unique IDs
const EventType ActorDestroyedEvent::s_eventType = 0x9a33c1b7;
const EventType ActorMovedEvent::s_eventType = 0x2b168326;
//========================================================================
ActorDestroyedEvent::ActorDestroyedEvent() :
m_id(-1)
{

}
// -----------------------------------------------------------------------
ActorDestroyedEvent::ActorDestroyedEvent(ActorID id) :
m_id(id)
{
}
// -----------------------------------------------------------------------
ActorDestroyedEvent::~ActorDestroyedEvent()
{

}
// -----------------------------------------------------------------------
void ActorDestroyedEvent::SetActorID(ActorID id)
{
	m_id = id;
}
// -----------------------------------------------------------------------
EventType ActorDestroyedEvent::VGetEventType() const
{
	return s_eventType; 
}
//========================================================================
ActorMovedEvent::ActorMovedEvent() :
m_velocity(0),
m_id(-1),
m_angularVelocity(0),
m_directional(false),
m_angular(false)
{
}
// -----------------------------------------------------------------------
ActorMovedEvent::ActorMovedEvent(ActorID id) :
m_velocity(0),
m_id(id),
m_angularVelocity(0),
m_directional(false),
m_angular(false)
{
}
// -----------------------------------------------------------------------
ActorMovedEvent::~ActorMovedEvent()
{

}
// -----------------------------------------------------------------------
void ActorMovedEvent::SetVelocity(vec3& relativeVector)
{
	m_velocity = relativeVector;
	m_directional = true;
}
// -----------------------------------------------------------------------
void ActorMovedEvent::SetAngularVelocity(vec3& relativeVector)
{
	m_angularVelocity = relativeVector;
	m_angular = true;
}
// -----------------------------------------------------------------------
EventType ActorMovedEvent::VGetEventType() const
{
	return s_eventType;
}
// -----------------------------------------------------------------------