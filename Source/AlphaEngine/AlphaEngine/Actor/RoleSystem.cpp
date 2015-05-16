#include "RoleSystem.h"
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
		}
		pElement = pElement->NextSiblingElement();
	}
	return true;
}
// -----------------------------------------------------------------------
StrongActorPtr RoleSystem::GetActor(ActorID id)
{
	return NULL;
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