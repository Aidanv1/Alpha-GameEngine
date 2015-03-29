#include "GameObjectFactory.h"

GameObjectFactory::GameObjectFactory() :
m_nextObjectID(0)
{

}
GameObjectFactory::~GameObjectFactory()
{

}
SharedGameObjectPtr GameObjectFactory::CreateGameObject(TiXmlElement* pElement)
{
	SharedGameObjectPtr pGameObject(ALPHA_NEW GameObject());
	TiXmlElement* componentElement = pElement->FirstChildElement();
	if (!componentElement)
	{
		//assert if gameobject element has no components
		ALPHA_ASSERT(false);
		return NULL;
	}
	while (componentElement)
	{
		//create component
		SharedGameObjectComponentPtr pComponent = CreateComponent(componentElement);
		if (!pComponent)
		{
			ALPHA_ASSERT(false);
			return NULL;
		}
		//add component to actor
		pComponent->SetOwner(pGameObject);
		pGameObject->AddComponent(pComponent);
		componentElement = componentElement->NextSiblingElement();
	}
	pGameObject->Init(GetNextGameObjectID());
	pGameObject->PostInit();
	return pGameObject;


}
GameObjectID GameObjectFactory::GetNextGameObjectID()
{
	return ++m_nextObjectID;
}
SharedGameObjectComponentPtr GameObjectFactory::CreateComponent(TiXmlElement* pElement)
{
	string componentType = pElement->Value();
	IComponentCreator* pCreator = m_componentCreatorMap[componentType].get();
	if (!pCreator)
	{
		//Error
	}
	IGameObjectComponent* pComponent = pCreator->CreateComponent(pElement);
	//Initialize component
	if (!pComponent->VInit(pElement))
	{
		//Error
	}
	 
	SharedGameObjectComponentPtr newComponent(pComponent);
	return newComponent;
}

bool GameObjectFactory::AddComponentCreator(IComponentCreator* pComponentCreator, string id)
{
	m_componentCreatorMap[id] = StrongComponentCreatorPtr(pComponentCreator);
	return true;
}