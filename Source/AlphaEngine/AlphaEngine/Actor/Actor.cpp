#include "Actor.h"
#include "..\AlphaStd.h"
Actor::Actor() :
m_ID(0),
m_name(""),
m_components()
{

}
// -----------------------------------------------------------------------
Actor::~Actor()
{

}
// -----------------------------------------------------------------------
bool Actor::Init(ActorID id)
{
	m_ID = id;
	return true;
}
// -----------------------------------------------------------------------
bool Actor::PostInit()
{
	//call post init for all components

	for (auto componentIt = m_components.begin(); componentIt != m_components.end(); componentIt++)
	{
		componentIt->second->VPostInit();
	}
	return true;
}
// -----------------------------------------------------------------------
bool Actor::AddComponent(StrongActorComponentPtr pComponent)
{
	m_components[pComponent->VGetType()] = pComponent;
	return true;
}
// -----------------------------------------------------------------------
void Actor::Update(float deltaMs)
{
	//Updates in reverse alphabetical order
	//Naming component types correctly is important!
	for (auto componentIt = m_components.rbegin(); componentIt != m_components.rend(); componentIt++)
	{
		IActorComponent* component =(componentIt->second).get();
		if (component)
		{
			component->VUpdate(deltaMs);
		}
	}
}
// -----------------------------------------------------------------------
IActorComponent* Actor::GetComponent(ComponentType type)
{
	auto findIt = m_components.find(type);
	if (findIt != m_components.end())
	{
		return m_components[type].get();
	}
	return NULL;
}
// -----------------------------------------------------------------------
void Actor::Destroy()
{
	m_components.clear();
}