#pragma once
#include "../AlphaStd.h"
#include "Actor.h"
#include "ActorFactory.h"
typedef map<ActorID, StrongActorPtr> ActorMap;
typedef map<string, StrongActorPtr> ActorNameMap;
//========================================================================
class RoleSystem
{
public:	
	~RoleSystem();
	bool Populate(TiXmlElement* pRoot);
	static RoleSystem& Get();
	StrongActorPtr GetActor(ActorID id) const;
	StrongActorPtr GetActor(string name) const;
private:
	RoleSystem();
	bool Init();
	ActorMap		m_actorRegistry;
	ActorNameMap	m_actorNameMap;
	ActorFactory	m_actorFactory;
	static bool m_initialized;
};
//========================================================================
