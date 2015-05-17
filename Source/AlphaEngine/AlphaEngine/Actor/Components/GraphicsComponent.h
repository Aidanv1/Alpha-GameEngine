#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H
#include "../IActorComponent.h"
class GraphicsComponent : public IActorComponent
{
	virtual ComponentType VGetType() const override{ return "Graphics"; };
};

#endif