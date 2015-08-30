#pragma once
#include "../EventManager/Events/Events.h"
#include "../Maths/GameMaths.h"
#include "../Actor/Actor.h"
//========================================================================
//Event Unique IDs
#define EVENT_PHYSICS_NEWCOLLISION 0x42799D4B
#define EVENT_PHYSICS_REMOVEDCOLLISION 0x2ea19f5f
#define EVENT_PHYSICS_LOADED 0xf16a9698
//========================================================================
using namespace std;
#include <set>
//========================================================================
//The contact information is stored as an average of all actual contact points
//as this is sufficient information in most games.
// -----------------------------------------------------------------------
struct CollisionObjectInfo
{
	ActorID		m_actorID;
	vec3		m_contactPoint;		//average of all contact points
	vec3		m_contactNormal;	//average of all contact normals
	CollisionObjectInfo(ActorID id, vec3& point, vec3& normal) :
		m_actorID(id),
		m_contactPoint(point),
		m_contactNormal(normal)
	{
	}

	CollisionObjectInfo() :
		m_actorID(0),
		m_contactPoint(0),
		m_contactNormal(0)
	{
	}
};
typedef pair<CollisionObjectInfo, CollisionObjectInfo> NewActorCollisionPair;
// -----------------------------------------------------------------------
class NewCollisionEvent : public BaseEvent
{
public:
	NewCollisionEvent(ActorID id1, ActorID id2,
					vec3& contactPoint1, vec3& contactPoint2,
					vec3& contactNormal1, vec3& contactNormal2);
	NewCollisionEvent(CollisionObjectInfo info1, CollisionObjectInfo info2);
	~NewCollisionEvent();
	NewActorCollisionPair GetCollisionPair() { return m_collidedActorsInfo; }
	EventType VGetEventType() const override { return EVENT_PHYSICS_NEWCOLLISION; }
private:
	NewActorCollisionPair m_collidedActorsInfo;
};
//========================================================================
typedef pair<ActorID, ActorID> RemovedActorCollisionPair;
// -----------------------------------------------------------------------
class RemovedCollisionEvent : public BaseEvent
{
public:
	RemovedCollisionEvent(ActorID id1, ActorID id2);
	~RemovedCollisionEvent();
	RemovedActorCollisionPair GetCollisionPair() { return m_collidedActorsInfo; }
	EventType VGetEventType() const override { return EVENT_PHYSICS_REMOVEDCOLLISION; }
private:
	RemovedActorCollisionPair m_collidedActorsInfo;
};
//========================================================================
class PhysicsLoadedEvent : public BaseEvent
{
public:
	PhysicsLoadedEvent();
	~PhysicsLoadedEvent();
	EventType VGetEventType() const override { return EVENT_PHYSICS_LOADED; }
};
//========================================================================
