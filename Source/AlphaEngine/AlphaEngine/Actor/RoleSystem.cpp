#include "../AlphaStd.h"
#include "RoleSystem.h"
//Include component creators
#include "ComponentCreators\GraphicsComponentCreator.h"
#include "ComponentCreators\TransformComponentCreator.h"
#include "ComponentCreators\PhysicsComponentCreator.h"
#include "Components\PhysicsComponent.h"
#include "Components\TransformComponent.h"
#include "ActorEvents.h"
bool RoleSystem::m_initialized = false;
// -----------------------------------------------------------------------
RoleSystem::RoleSystem() :
m_actorRegistry(),
m_actorFactory()
{

}
// -----------------------------------------------------------------------
RoleSystem::~RoleSystem()
{
	m_actorRegistry.clear();
	Deregister_Listener(ActorDestroyedEvent::s_eventType, this, &RoleSystem::ActorDestroyedDelegate);
	Deregister_Listener(ActorMovedEvent::s_eventType, this, &RoleSystem::ActorMovedDelegate);
}
// -----------------------------------------------------------------------
bool RoleSystem::Init()
{
	Register_Listener(ActorDestroyedEvent::s_eventType, this, &RoleSystem::ActorDestroyedDelegate);
	Register_Listener(ActorMovedEvent::s_eventType, this, &RoleSystem::ActorMovedDelegate);
	//add component creators
	StrongComponentCreatorPtr graphicsComponentCreator(ALPHA_NEW GraphicsComponentCreator());
	m_actorFactory.AddComponentCreator(graphicsComponentCreator, "Graphics");
	//
	StrongComponentCreatorPtr transformComponentCreator(ALPHA_NEW TransformComponentCreator());
	m_actorFactory.AddComponentCreator(transformComponentCreator, "Transform");
	//
	StrongComponentCreatorPtr physicsComponentCreator(ALPHA_NEW PhysicsComponentCreator());
	m_actorFactory.AddComponentCreator(physicsComponentCreator, "Physics");
	return true;
}
// -----------------------------------------------------------------------
bool RoleSystem::Populate(TiXmlElement* pRoot)
{
	TiXmlElement* pElement = pRoot->FirstChildElement();

	while (pElement)
	{
		string value;
		value = pElement->Value();
		if (value == "Actor")
		{
			StrongActorPtr actor = m_actorFactory.CreateActor(pElement);
			m_actorRegistry[actor->GetID()] = actor;
			m_actorNameMap[actor->GetName()] = actor;
		}
		pElement = pElement->NextSiblingElement();
	}
	return true;
}
// -----------------------------------------------------------------------
RoleSystem& RoleSystem::Get()
{
	static RoleSystem* instance = ALPHA_NEW RoleSystem();
	if (!m_initialized)
	{
		instance->Init();
	}
	m_initialized = true;
	return *instance;
}
// -----------------------------------------------------------------------
Actor* RoleSystem::GetActor(ActorID id) const
{
	auto findIt = m_actorRegistry.find(id);
	if (findIt != m_actorRegistry.end())
	{
		return (*findIt).second.get();
	}
	return NULL;
}
// -----------------------------------------------------------------------
Actor* RoleSystem::GetActor(string name) const
{
	auto findIt = m_actorNameMap.find(name);
	if (findIt != m_actorNameMap.end())
	{
		return (*findIt).second.get();
	}
	return NULL;
}
// -----------------------------------------------------------------------
void RoleSystem::Update(float deltaMs)
{
	for (auto actorIt = m_actorRegistry.begin(); actorIt != m_actorRegistry.end(); actorIt++)
	{
		(*actorIt).second->Update(deltaMs);
	}
}
// -----------------------------------------------------------------------
void RoleSystem::ActorDestroyedDelegate(StrongEventPtr e)
{
	ActorDestroyedEvent* desEvent = dynamic_cast<ActorDestroyedEvent*>(e.get());
	if (!desEvent)
	{
		return;
	}
	auto id = desEvent->GetActorID();
	//if actor exists, destroy it
	auto findIt = m_actorRegistry.find(id);
	if (findIt != m_actorRegistry.end())
	{
		Actor* a = (*findIt).second.get();
		a->Destroy();
		m_actorRegistry.erase(findIt);
	}
}
// -----------------------------------------------------------------------
void RoleSystem::ActorMovedDelegate(StrongEventPtr e)
{
	//only move actor if it has the neccessary components
	ActorMovedEvent* moveEvent = dynamic_cast<ActorMovedEvent*>(e.get());
	if (!moveEvent)
	{
		return;
	}
	auto id = moveEvent->GetActorID();
	auto findIt = m_actorRegistry.find(id);
	if (findIt == m_actorRegistry.end())
	{
		//actor doesnt exist
		return;
	}
	Actor* a = (*findIt).second.get();
	PhysicsComponent* physCom = dynamic_cast<PhysicsComponent*>(a->GetComponent("Physics"));
	TransformComponent* transCom = dynamic_cast<TransformComponent*>(a->GetComponent("Transform"));
	if (!physCom)
	{
		//actor does not have a physics component
		return;
	}
	if (moveEvent->IsDirectional())
	{
		vec3 vel = moveEvent->GetVelocity();
		mat4 rot = *transCom->GetRotation().Get();
		vec4 velTemp = vec4(vel, 1);
		velTemp = rot * velTemp;
		vel = vec3(velTemp.x, velTemp.y, velTemp.z);
		physCom->SetVelocity(vel);
	}
	if (moveEvent->IsAngular())
	{
		physCom->SetAngularVelocity(moveEvent->GetAngularVelocity());
	}
}