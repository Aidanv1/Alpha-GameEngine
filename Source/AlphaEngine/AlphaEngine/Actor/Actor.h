#pragma once
#include "IActorComponent.h"
#include <memory>
#include <map>
#include <string>
class Actor;
typedef unsigned long ActorID;
typedef std::shared_ptr<Actor> StrongActorPtr;
typedef std::shared_ptr<IActorComponent> StrongActorComponentPtr;
typedef std::map<ComponentType, StrongActorComponentPtr> ComponentMap;
//========================================================================
class DLLExport Actor
{
	friend class ActorFactory;
	friend class RoleSystem;
public:
	Actor();
	~Actor();	
	void Destroy();
	ActorID GetID() const { return m_ID; }
	std::string GetName() const { return m_name; }
	IActorComponent* GetComponent(ComponentType);
private:
	void Update(float deltaMs);
	bool AddComponent(StrongActorComponentPtr);
	bool Init(ActorID id);
	bool PostInit();
private:
	ActorID m_ID;
	ComponentMap m_components;
	std::string m_name;
};
//========================================================================
