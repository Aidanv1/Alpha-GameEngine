#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "..\AlphaStd.h"
#include "IGameObjectComponent.h"

class GameObject;
typedef unsigned long GameObjectID;
typedef shared_ptr<GameObject> SharedGameObjectPtr;
typedef shared_ptr<IGameObjectComponent> SharedGameObjectComponentPtr;
typedef map<ComponentType, SharedGameObjectComponentPtr> ComponentMap;



class GameObject
{
	friend class GameObjectFactory;
public:
	GameObject();
	~GameObject();
	bool Init(GameObjectID id);
	bool PostInit();
	void Update();
	void Destroy();
	GameObjectID GetID() const { return m_ID; }
	IGameObjectComponent* GetComponent(ComponentType);
private:
	bool AddComponent(SharedGameObjectComponentPtr);
private:
	GameObjectID m_ID;
	ComponentMap m_components;
};

#endif