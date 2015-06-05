#pragma once
#include "../IComponentCreator.h"
#include "../../Graphics3D/Scene/ModelNode.h"
#include "../../Graphics3D/OpenGL/DrawableNodes/SkyBox_GL.h"
#include "../../Graphics3D/Scene/CameraNode.h"
#include "../../Graphics3D/OpenGL/DrawableNodes/HeightMap_GL.h"
class GraphicsComponentCreator : public IComponentCreator
{
public:
	virtual IActorComponent* CreateComponent(TiXmlElement* pElement) override;
};
