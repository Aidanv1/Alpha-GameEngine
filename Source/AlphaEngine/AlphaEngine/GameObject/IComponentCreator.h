#ifndef ICOMPONENTCREATOR_H
#define ICOMPONENTCREATOR_H
#include "IGameObjectComponent.h"
//#include "..\AlphaStd.h"
class IComponentCreator
{
public:
	virtual IGameObjectComponent* CreateComponent(TiXmlElement* pElement) = 0;
};

#endif