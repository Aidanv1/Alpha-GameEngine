#pragma once
#include "../AlphaEngineDLL.h"
#include <memory>
#include <string>
typedef std::string ComponentType;
class Actor;
class TiXmlElement;
//========================================================================
class DLLExport IActorComponent
{
	friend class ActorFactory;
protected:
	std::shared_ptr<Actor> m_pOwner;
public:
	virtual ComponentType VGetType() const = 0;
	virtual void VUpdate(float deltaMs) = 0;
	virtual bool VInitComponent(TiXmlElement* pElement) = 0;
	virtual bool VPostInit() = 0;
private:
	void SetOwner(std::shared_ptr<Actor> pOwner){ m_pOwner = pOwner; }
};
//========================================================================
