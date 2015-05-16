#ifndef Actor_H
#define Actor_H
#include "..\AlphaStd.h"
#include "IActorComponent.h"

class Actor;
typedef unsigned long ActorID;
typedef shared_ptr<Actor> StrongActorPtr;
typedef shared_ptr<IActorComponent> StrongActorComponentPtr;
typedef map<ComponentType, StrongActorComponentPtr> ComponentMap;
//========================================================================
class Actor
{
	friend class ActorFactory;
public:
	Actor();
	~Actor();
	bool Init(ActorID id);
	bool PostInit();
	void Update();
	void Destroy();
	ActorID GetID() const { return m_ID; }
	IActorComponent* GetComponent(ComponentType);
private:
	bool AddComponent(StrongActorComponentPtr);
private:
	ActorID m_ID;
	ComponentMap m_components;
};
//========================================================================
#endif