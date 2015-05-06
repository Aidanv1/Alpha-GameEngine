#ifndef IGAMEOBJECTCOMPONENT
#define IGAMEOBJECTCOMPONENT
#include "..\AlphaStd.h"
#include "GameObject.h"

typedef unsigned long ComponentType;
class GameObject;
class IGameObjectComponent
{
	friend class GameObjectFactory;
protected:
	shared_ptr<GameObject> m_pOwner;
public:
	virtual ComponentType VGetType() = 0;
	virtual void VUpdate() = 0;
	virtual bool VInit(TiXmlElement* pElement) = 0;
	virtual bool VPostInit() = 0;
private:
	void SetOwner(shared_ptr<GameObject> pOwner){ m_pOwner = pOwner; }
};
#endif