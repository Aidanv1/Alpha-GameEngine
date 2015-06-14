#pragma once
#include "..\AlphaStd.h"
typedef string ComponentType;
class Actor;
//========================================================================
class IActorComponent
{
	friend class ActorFactory;
protected:
	shared_ptr<Actor> m_pOwner;
public:
	virtual ComponentType VGetType() const = 0;
	virtual void VUpdate(float deltaMs) = 0;
	virtual bool VInitComponent(TiXmlElement* pElement) = 0;
	virtual bool VPostInit() = 0;
private:
	void SetOwner(shared_ptr<Actor> pOwner){ m_pOwner = pOwner; }
};
//========================================================================
