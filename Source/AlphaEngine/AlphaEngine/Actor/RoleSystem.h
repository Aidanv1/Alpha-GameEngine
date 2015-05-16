#ifndef ROLESYSTEM_H
#define ROLESYSTEM_H
#include "../AlphaStd.h"
#include "Actor.h"
#include "ActorFactory.h"
typedef map<ActorID, StrongActorPtr> ActorMap;
//========================================================================
class RoleSystem
{
public:	
	~RoleSystem();
	bool Init(TiXmlElement pRoot);
	static RoleSystem& Get();
	StrongActorPtr GetActor(ActorID id);
private:
	RoleSystem();
	ActorMap m_actorRegistry;
	ActorFactory m_actorFactory;
};
//========================================================================
#endif