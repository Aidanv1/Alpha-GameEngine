#pragma once
#include "../EventManager/Events/Events.h"
#include "../AlphaEngineDLL.h"
#include "../Maths/GameMaths.h"
#include "Actor.h"
//========================================================================
class DLLExport ActorDestroyedEvent : public BaseEvent
{
public:
	ActorDestroyedEvent();
	ActorDestroyedEvent(ActorID id);
	~ActorDestroyedEvent();
	void SetActorID(ActorID id);
	ActorID GetActorID() const { return m_id; };
	virtual EventType VGetEventType() const override;
public:
	static const EventType s_eventType;
private:
	ActorID m_id;
};
//========================================================================
class DLLExport ActorMovedEvent : public BaseEvent
{
public:
	ActorMovedEvent();
	ActorMovedEvent(ActorID id);
	~ActorMovedEvent();
	void SetVelocity(vec3& relativeVector);
	void SetAngularVelocity(vec3& relativeVector);
	bool IsDirectional(){ return m_directional; }
	bool IsAngular(){ return m_angular; }
	vec3 GetAngularVelocity() { return m_angularVelocity; }
	vec3 GetVelocity() { return m_velocity; }
	ActorID GetActorID() const { return m_id; };
	virtual EventType VGetEventType() const override;
public:
	static const EventType s_eventType;
private:
	vec3	m_velocity;
	vec3    m_angularVelocity;
	ActorID m_id;
	bool	m_directional;
	bool	m_angular;
};
//========================================================================
class DLLExport ActorJumpedEvent : public BaseEvent
{
public:
	ActorJumpedEvent();
	ActorJumpedEvent(ActorID id, vec3 force);
	~ActorJumpedEvent();
	void SetForce(vec3 force);
	vec3 GetForce() { return m_force; }
	ActorID GetActorID() const { return m_id; };
	virtual EventType VGetEventType() const override;
public:
	static const EventType s_eventType;
private:
	vec3	m_force;
	ActorID m_id;
};
//========================================================================
