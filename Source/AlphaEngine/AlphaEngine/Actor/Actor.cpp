#include "Actor.h"
Actor::Actor() :
m_ID(0),
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
void Actor::Update()
{
	for (auto componentIt = m_components.begin(); componentIt != m_components.end(); componentIt++)
	{
		componentIt->second->VUpdate();
	}
}
// -----------------------------------------------------------------------
IActorComponent* Actor::GetComponent(ComponentType type)
{
	return m_components[type].get();
}
// -----------------------------------------------------------------------
void Actor::Destroy()
{
	m_components.clear();
}