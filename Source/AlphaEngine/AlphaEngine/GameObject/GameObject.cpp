#include "GameObject.h"
GameObject::GameObject() :
m_ID(0),
m_components()
{

}

GameObject::~GameObject()
{

}

bool GameObject::Init(GameObjectID id)
{
	m_ID = id;
	return true;
}

bool GameObject::PostInit()
{
	//call post init for all components

	for (auto componentIt = m_components.begin(); componentIt != m_components.end(); componentIt++)
	{
		componentIt->second->VPostInit();
	}
	return true;
}

bool GameObject::AddComponent(SharedGameObjectComponentPtr pComponent)
{
	m_components[pComponent->VGetType()] = pComponent;
	return true;
}

void GameObject::Update()
{
	for (auto componentIt = m_components.begin(); componentIt != m_components.end(); componentIt++)
	{
		componentIt->second->VUpdate();
	}
}

IGameObjectComponent* GameObject::GetComponent(ComponentType type)
{
	return m_components[type].get();
}

void GameObject::Destroy()
{
	m_components.clear();
}