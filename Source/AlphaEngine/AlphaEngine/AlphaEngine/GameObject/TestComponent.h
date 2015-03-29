#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H
#include "IGameObjectComponent.h"
class TestComponent : public IGameObjectComponent
{
public:
	virtual ComponentType VGetType() override
	{
		return s_type;
	}
	string GetName()
	{
		return m_name;
	}
	unsigned long GetOwnerId()
	{
		return m_pOwner->GetID();
	}
	string SetName(const char* s)
	{
		return m_name = s;
	}
	virtual void VUpdate() override
	{

	}
	bool VInit(TiXmlElement* pElement) override
	{
		cout << "Init" << endl;
		return true;
	}
	bool VPostInit() override
	{
		cout << "POST" << endl;
		return true;
	}
private:
	string m_name;
	static const ComponentType s_type = 1;
};
#endif