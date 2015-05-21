#pragma once
#include "../IActorComponent.h"
class GraphicsComponent : public IActorComponent
{
public:
	virtual ComponentType VGetType() const override{ return "Graphics"; }
	virtual bool VPostInit() override;
};

