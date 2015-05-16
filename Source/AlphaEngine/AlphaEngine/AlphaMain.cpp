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

int main(int argc, char *argv[])
{
	IWindow* window = ALPHA_NEW GLWindow("AlphaEngine", 1280, 720);
	GameLoop gameLoop;
	gameLoop.Init(window);
	//Model creation example ----------
	ModelComponentCreator mcc;
	SkyBoxComponentCreator scc;
	//Load xml
	TiXmlDocument doc;
	doc.LoadFile("hello.xml");
	//scenenode
	//first model----
	TiXmlElement* elem = doc.FirstChildElement();
	elem = elem->FirstChildElement();
	Model* node = (Model*)mcc.CreateComponent(elem);
	elem = elem->NextSiblingElement();
	SkyBox* sky = (SkyBox*)scc.CreateComponent(elem);
	//camera
	Camera* camera = new Camera(45.0f, 1.7778f, 0.1f, 10000.0f, Orbital_Mode, node);
	
	camera->VInitNode();
	GraphicsSystem::Get().GetScene()->SetCameraNode(shared_ptr<Camera>(camera));

	GraphicsSystem::Get().GetScene()->AddChild(shared_ptr<SceneNode>(node));

	GraphicsSystem::Get().GetScene()->AddChild(shared_ptr<SceneNode>(sky));

	GraphicsSystem::Get().LoadScene();

	gameLoop.StartLoop();
	return 0;
	// -----------------------------------------------------------------------
}
	
