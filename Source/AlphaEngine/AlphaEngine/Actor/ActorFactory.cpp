#include "ActorFactory.h"
// -----------------------------------------------------------------------
ActorFactory::ActorFactory() :
m_nextObjectID(0)
{

}
// -----------------------------------------------------------------------
ActorFactory::~ActorFactory()
{

}
// -----------------------------------------------------------------------
StrongActorPtr ActorFactory::CreateActor(TiXmlElement* pElement)
{
	StrongActorPtr pActor(ALPHA_NEW Actor());
	TiXmlElement* componentElement = pElement->FirstChildElement();
	if (!componentElement)
	{
		//assert if Actor element has no components
		ALPHA_ASSERT(false);
		return NULL;
	}
	while (componentElement)
	{
		//create component
		StrongActorComponentPtr pComponent = CreateComponent(componentElement);
		if (!pComponent)
		{
			ALPHA_ASSERT(false);
			return NULL;
		}
		//add component to actor
		pComponent->SetOwner(pActor);
		pActor->AddComponent(pComponent);
		componentElement = componentElement->NextSiblingElement();
	}
	pActor->Init(GetNextActorID());
	pActor->PostInit();
	return pActor;
}
// -----------------------------------------------------------------------
ActorID ActorFactory::GetNextActorID()
{
	return ++m_nextObjectID;
}
// -----------------------------------------------------------------------
StrongActorComponentPtr ActorFactory::CreateComponent(TiXmlElement* pElement)
{
	string componentType = pElement->Value();
	IComponentCreator* pCreator = m_componentCreatorMap[componentType].get();
	if (!pCreator)
	{
		//Error
	}
	IActorComponent* pComponent = pCreator->CreateComponent(pElement);
	//Initialize component
	if (!pComponent->VInitComponent(pElement))
	{
		//Error
	}
	 
	StrongActorComponentPtr newComponent(pComponent);
	return newComponent;
}
// -----------------------------------------------------------------------
bool ActorFactory::AddComponentCreator(IComponentCreator* pComponentCreator, string id)
{
	m_componentCreatorMap[id] = StrongComponentCreatorPtr(pComponentCreator);
	return true;
}
// -----------------------------------------------------------------------