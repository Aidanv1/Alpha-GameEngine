#pragma once
#include "..\AlphaStd.h"
#include "Actor.h"
#include "IComponentCreator.h"

typedef shared_ptr<IComponentCreator> StrongComponentCreatorPtr;
//========================================================================
class ActorFactory
{
public:
	ActorFactory();
	~ActorFactory();
	StrongActorPtr CreateActor(TiXmlElement* pElement);
	bool AddComponentCreator(StrongComponentCreatorPtr pComponentCreator, string id);
private:
	ActorID GetNextActorID();
	StrongActorComponentPtr CreateComponent(TiXmlElement* pElement);
private:
	ActorID m_nextObjectID;
	map<string, StrongComponentCreatorPtr> m_componentCreatorMap;

};
//========================================================================
