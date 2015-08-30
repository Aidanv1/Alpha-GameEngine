#include "PhysicsComponentCreator.h"
#include "../Components/PhysicsComponent.h"
// -----------------------------------------------------------------------
IActorComponent* PhysicsComponentCreator::CreateComponent(TiXmlElement* pElement)
{
	PhysicsComponent* component = ALPHA_NEW PhysicsComponent();
	return component;
}
// -----------------------------------------------------------------------