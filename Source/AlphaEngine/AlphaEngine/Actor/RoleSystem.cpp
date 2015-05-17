#include "RoleSystem.h"
//Include component creators
#include "ComponentCreators\GraphicsComponentCreator.h"
bool RoleSystem::m_initialized = false;
// -----------------------------------------------------------------------
RoleSystem::RoleSystem() :
m_actorRegistry(),
m_actorFactory()
{

}
// -----------------------------------------------------------------------
RoleSystem::~RoleSystem()
{
	m_actorRegistry.clear();
}
// -----------------------------------------------------------------------
bool RoleSystem::Init()
{
	//add component creators
	StrongComponentCreatorPtr graphicsComponentCreator(ALPHA_NEW GraphicsComponentCreator());
	m_actorFactory.AddComponentCreator(graphicsComponentCreator, "Graphics");
	return true;
}
// -----------------------------------------------------------------------
bool RoleSystem::Populate(TiXmlElement* pRoot)
{
	TiXmlElement* pElement = pRoot->FirstChildElement();

	while (pElement)
	{
		string value;
		value = pElement->Value();
		if (value == "Actor")
		{
			StrongActorPtr actor = m_actorFactory.CreateActor(pElement);
			m_actorRegistry[actor->GetID()] = actor;
			m_actorNameMap[actor->GetName()] = actor;
		}
		pElement = pElement->NextSiblingElement();
	}
	return true;
}
// -----------------------------------------------------------------------
RoleSystem& RoleSystem::Get()
{
	static RoleSystem* instance = ALPHA_NEW RoleSystem();
	if (!m_initialized)
	{
		instance->Init();
	}
	m_initialized = true;
	return *instance;
}
// -----------------------------------------------------------------------
StrongActorPtr RoleSystem::GetActor(ActorID id) const
{
	auto findIt = m_actorRegistry.find(id);
	if (findIt != m_actorRegistry.end())
	{
		return (*findIt).second;
	}
	return NULL;
}
// -----------------------------------------------------------------------
StrongActorPtr RoleSystem::GetActor(string name) const
{
	auto findIt = m_actorNameMap.find(name);
	if (findIt != m_actorNameMap.end())
	{
		return (*findIt).second;
	}
	return NULL;
}
// -----------------------------------------------------------------------