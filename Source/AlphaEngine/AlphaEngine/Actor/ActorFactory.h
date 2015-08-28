#pragma once
#include "Actor.h"
#include "IComponentCreator.h"
//Forward declarations
class TiXmlElement;
//
typedef std::shared_ptr<IComponentCreator> StrongComponentCreatorPtr;
//========================================================================
class ActorFactory
{
public:
	ActorFactory();
	~ActorFactory();
	StrongActorPtr CreateActor(TiXmlElement* pElement);
	bool AddComponentCreator(StrongComponentCreatorPtr pComponentCreator, std::string id);
private:
	ActorID GetNextActorID();
	StrongActorComponentPtr CreateComponent(TiXmlElement* pElement);
private:
	ActorID m_nextObjectID;
	std::map<std::string, StrongComponentCreatorPtr> m_componentCreatorMap;

};
//========================================================================
