#ifndef TESTCOMPONENTCREATOR_H
#define TESTCOMPONENTCREATOR_H
#include "IComponentCreator.h"
#include "TestComponent.h"
#include "..\AlphaStd.h"
class TestComponentCreator : public IComponentCreator
{
	IGameObjectComponent* CreateComponent(TiXmlElement* pElement) override
	{
		TestComponent* pComponent = new TestComponent();
		const char* name = pElement->Attribute("name");
		pComponent->SetName(name);
		return pComponent;
	}
};
#endif