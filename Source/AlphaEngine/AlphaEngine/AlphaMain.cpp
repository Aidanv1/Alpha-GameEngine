#include "EventManager\EventManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>	
#include <glm/gtx/transform.hpp>
#include "Graphics3D\OpenGL\Data\Texture.h"
#include "ResourceManager\Loaders\BitmapResourceLoader.h"
#include "ResourceManager\Loaders\MeshResourceLoader.h"
#include "Graphics3D\GraphicsSystem.h"
#include "Graphics3D\Scene\Model.h"
#include "Graphics3D\OpenGL\Data\Material.h"
#include "Graphics3D\OpenGL\Data\Texture.h"
#include "GameLoop\GameLoop.h"
#include "Graphics3D\Scene\Camera.h"
#include "Graphics3D/OpenGL/Text/Text2D_GL.h"
#include "Graphics3D\OpenGL\Data\SkyBox.h"
#include "Actor/RoleSystem.h"

int main(int argc, char *argv[])
{
	IWindow* window = ALPHA_NEW GLWindow("AlphaEngine", 1280, 720);
	GameLoop gameLoop;
	gameLoop.Init(window);
	//Load xml
	TiXmlDocument doc;
	doc.LoadFile("Actors.xml");
	TiXmlElement* elem = doc.FirstChildElement();
	RoleSystem::Get().Populate(elem);
	GraphicsSystem::Get().LoadScene();

	gameLoop.StartLoop();
	return 0;
	// -----------------------------------------------------------------------
}
	
