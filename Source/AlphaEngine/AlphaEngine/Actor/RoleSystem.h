#ifndef ROLESYSTEM_H
#define ROLESYSTEM_H
#include "../AlphaStd.h"
#include "Actor.h"
#include "ActorFactory.h"

#include "ComponentCreators\GraphicsComponentCreator.h"
typedef map<ActorID, StrongActorPtr> ActorMap;
//========================================================================
class RoleSystem
{
public:	
	~RoleSystem();
	bool Populate(TiXmlElement* pRoot);
	static RoleSystem& Get();
	StrongActorPtr GetActor(ActorID id);
private:
	RoleSystem();
	bool Init();
	ActorMap		m_actorRegistry;
	ActorFactory	m_actorFactory;
	static bool m_initialized;
};
//========================================================================
#endif