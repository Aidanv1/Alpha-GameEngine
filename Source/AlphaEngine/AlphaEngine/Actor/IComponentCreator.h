#pragma once
#include "IActorComponent.h"
//========================================================================
class IComponentCreator
{
public:
	virtual IActorComponent* CreateComponent(TiXmlElement* pElement) = 0;
};
//========================================================================
