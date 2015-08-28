#include "TransformComponentCreator.h"
#include "../../AlphaStd.h"
IActorComponent* TransformComponentCreator::CreateComponent(TiXmlElement* pElement)
{
	TransformComponent* component = ALPHA_NEW TransformComponent();
	TiXmlElement* nextElem = pElement->FirstChildElement();
	component->VInitComponent(nextElem);
	return component;
}