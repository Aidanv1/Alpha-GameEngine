#pragma once
#include "../IComponentCreator.h"
//========================================================================
class ClonerComponentCreator : public IComponentCreator
{
public:
	virtual IActorComponent* CreateComponent(TiXmlElement* pElement) override;
};
//========================================================================