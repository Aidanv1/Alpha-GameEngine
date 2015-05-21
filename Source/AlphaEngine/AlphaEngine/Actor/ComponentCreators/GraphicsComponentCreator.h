#pragma once
#include "../IComponentCreator.h"
#include "../../Graphics3D/Scene/Model.h"
#include "../../Graphics3D/OpenGL/Data/SkyBox.h"
#include "../../Graphics3D/Scene/Camera.h"
#include "../../Graphics3D/OpenGL/Data/HeightMap.h"
class GraphicsComponentCreator : public IComponentCreator
{
public:
	virtual IActorComponent* CreateComponent(TiXmlElement* pElement) override;
};
