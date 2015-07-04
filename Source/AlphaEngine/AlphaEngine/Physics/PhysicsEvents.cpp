#include "PhysicsEvents.h"
//========================================================================
//Event Unique IDs
const EventType NewCollisionEvent::s_eventType = 0x42799D4B;
const EventType RemovedCollisionEvent::s_eventType = 0x2ea19f5f;
const EventType PhysicsLoadedEvent::s_eventType = 0xf16a9698;
//========================================================================
NewCollisionEvent::NewCollisionEvent(ActorID id1, ActorID id2,
									vec3& contactPoint1, vec3& contactPoint2,
									vec3& contactNormal1, vec3& contactNormal2)
{
	m_collidedActorsInfo = make_pair(	CollisionObjectInfo(id1, contactPoint1, contactNormal1), 
										CollisionObjectInfo(id2, contactPoint2, contactNormal2));
}
// -----------------------------------------------------------------------
NewCollisionEvent::NewCollisionEvent(CollisionObjectInfo info1, CollisionObjectInfo info2)
{
	m_collidedActorsInfo = make_pair(info1, info2);
}
// -----------------------------------------------------------------------
NewCollisionEvent::~NewCollisionEvent()
{
}
//========================================================================
RemovedCollisionEvent::RemovedCollisionEvent(ActorID id1, ActorID id2)
{
	m_collidedActorsInfo = make_pair(id1, id2);
}
// -----------------------------------------------------------------------
RemovedCollisionEvent::~RemovedCollisionEvent()
{
}
//========================================================================
PhysicsLoadedEvent::PhysicsLoadedEvent()
{
}
// -----------------------------------------------------------------------
PhysicsLoadedEvent::~PhysicsLoadedEvent()
{
}