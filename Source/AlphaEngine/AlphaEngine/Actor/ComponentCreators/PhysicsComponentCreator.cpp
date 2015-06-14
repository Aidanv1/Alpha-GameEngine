#include "PhysicsComponentCreator.h"
#include "../Components/PhysicsComponent.h"
// -----------------------------------------------------------------------
IActorComponent* PhysicsComponentCreator::CreateComponent(TiXmlElement* pElement)
{
	PhysicsComponent* component = ALPHA_NEW PhysicsComponent();
	component->VInitComponent(pElement);
	return component;
}
// -----------------------------------------------------------------------