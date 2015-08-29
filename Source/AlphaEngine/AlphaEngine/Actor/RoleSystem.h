#pragma once
#include "../AlphaEngineDLL.h"
#include <map>
#include "Actor.h"
#include "ActorFactory.h"
#include "../EventManager/IEventManager.h"
class TiXmlElement;
typedef std::map<ActorID, StrongActorPtr> ActorMap;
typedef std::map<std::string, StrongActorPtr> ActorNameMap;
//========================================================================
class RoleSystem
{
public:	
	~RoleSystem();
	bool Populate(TiXmlElement* pRoot);
	static RoleSystem& Get();
	DLLExport Actor* GetActor(ActorID id) const;
	DLLExport Actor* GetActor(std::string name) const;
	void Update(float deltaMs);
	//delegate listeners
	void ActorDestroyedDelegate(StrongEventPtr e);
	void ActorMovedDelegate(StrongEventPtr e);
	void ActorJumpedDelegate(StrongEventPtr e);
private:
	RoleSystem();
	bool Init();
	ActorMap		m_actorRegistry;
	ActorNameMap	m_actorNameMap;
	ActorFactory	m_actorFactory;
	static bool		m_initialized;
};
//========================================================================
