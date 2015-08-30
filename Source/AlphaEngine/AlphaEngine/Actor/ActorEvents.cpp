#include "ActorEvents.h"
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
	return EVENT_ACTOR_DESTROYED;
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
	return EVENT_ACTOR_MOVED;
}

//========================================================================
ActorJumpedEvent::ActorJumpedEvent() :
m_id(-1),
m_force(0)
{

}
// -----------------------------------------------------------------------
ActorJumpedEvent::ActorJumpedEvent(ActorID id, vec3 force) :
m_id(id),
m_force(force)
{

}
// -----------------------------------------------------------------------
ActorJumpedEvent::~ActorJumpedEvent()
{

}
// -----------------------------------------------------------------------
void ActorJumpedEvent::SetForce(vec3 force)
{
	m_force = force;
}
// -----------------------------------------------------------------------
EventType ActorJumpedEvent::VGetEventType() const
{
	return EVENT_ACTOR_JUMPED;
}
// -----------------------------------------------------------------------