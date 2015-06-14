#pragma once
#include "../IComponentCreator.h"
#include "../Components/TransformComponent.h"
class TransformComponentCreator : public IComponentCreator
{
public:
	virtual IActorComponent* CreateComponent(TiXmlElement* pElement) override;
};
