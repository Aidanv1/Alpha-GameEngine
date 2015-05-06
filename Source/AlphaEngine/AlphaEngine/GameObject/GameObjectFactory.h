#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H
#include "..\AlphaStd.h"
#include "GameObject.h"
#include "IComponentCreator.h"


typedef shared_ptr<IComponentCreator> StrongComponentCreatorPtr;
class GameObjectFactory
{
public:
	GameObjectFactory();
	~GameObjectFactory();
	SharedGameObjectPtr CreateGameObject(TiXmlElement* pElement);
	bool AddComponentCreator(IComponentCreator* pComponentCreator, string id);
private:
	GameObjectID GetNextGameObjectID();
	SharedGameObjectComponentPtr CreateComponent(TiXmlElement* pElement);
private:
	GameObjectID m_nextObjectID;
	map<string, StrongComponentCreatorPtr> m_componentCreatorMap;

};
#endif