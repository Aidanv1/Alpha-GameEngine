#ifndef IActorCOMPONENT
#define IActorCOMPONENT
#include "..\AlphaStd.h"


typedef unsigned long ComponentType;
class Actor;
//========================================================================
class IActorComponent
{
	friend class ActorFactory;
protected:
	shared_ptr<Actor> m_pOwner;
public:
	virtual ComponentType VGetType() = 0;
	virtual void VUpdate() = 0;
	virtual bool VInitComponent(TiXmlElement* pElement) = 0;
	virtual bool VPostInit() = 0;
private:
	void SetOwner(shared_ptr<Actor> pOwner){ m_pOwner = pOwner; }
};
//========================================================================
#endif