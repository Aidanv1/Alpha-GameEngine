#pragma once
#include "../IComponentCreator.h"
#include "../../Graphics3D/Scene/ModelNode.h"
#include "../../Graphics3D/Scene/CameraNode.h"
//========================================================================
class GraphicsComponentCreator : public IComponentCreator
{
public:
	virtual IActorComponent* CreateComponent(TiXmlElement* pElement) override;
};
//========================================================================