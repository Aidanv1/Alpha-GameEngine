#ifndef ICOMPONENTCREATOR_H
#define ICOMPONENTCREATOR_H
#include "IActorComponent.h"
//========================================================================
class IComponentCreator
{
public:
	virtual IActorComponent* CreateComponent(TiXmlElement* pElement) = 0;
};
//========================================================================
#endif