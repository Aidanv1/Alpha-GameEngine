#include "DrawableNode.h"
#include "../GraphicsSystem.h"
#include "../OpenGL/Renderer_GL.h"
#include "../OpenGL/DrawableNodes/Mesh_GL.h"
#include "../OpenGL/DrawableNodes/SkyBox_GL.h"
#include "../OpenGL/DrawableNodes/Text_GL.h"
// -----------------------------------------------------------------------
ISceneNode* DrawableNodeFactory::CreateDrawableNode(NodeType type)
{
	bool isOpenGL = (dynamic_cast<Renderer_GL*>(GraphicsSystem::Get().GetRenderer()))? true: false;
	
	switch (type)
	{
	case Node_Mesh:
		if (isOpenGL)
		{
			return ALPHA_NEW Mesh_GL();
		}
		break;
	case Node_Sky:
		if (isOpenGL)
		{
			return ALPHA_NEW SkyBox_GL();
		}
		break;	
	case Node_Text:
		if (isOpenGL)
		{
			return ALPHA_NEW Text_GL();
		}
		break;
	}
	return NULL;
}
// -----------------------------------------------------------------------