#pragma once
#include "../IComponentCreator.h"
//========================================================================
class PhysicsComponentCreator : public IComponentCreator
{
public:
	virtual IActorComponent* CreateComponent(TiXmlElement* pElement) override;
};
//========================================================================