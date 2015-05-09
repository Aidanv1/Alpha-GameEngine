#include "EventManager\EventManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>	
#include <glm/gtx/transform.hpp>
#include "Graphics\OpenGL\Data\Texture.h"
#include "ResourceManager\Loaders\BitmapResourceLoader.h"
#include "ResourceManager\Loaders\MeshResourceLoader.h"
#include "Graphics\GraphicsSystem.h"
#include "Graphics\ModelComponent.h"
#include "Graphics\OpenGL\Data\Material.h"
#include "Graphics\OpenGL\Data\Texture.h"
#include "GameLoop\GameLoop.h"
#include "Graphics/Camera.h"
#include "Memory"
int main(int argc, char *argv[])
{
	IWindow* window = ALPHA_NEW GLWindow("AlphaEngine", 1280, 720);
	GameLoop gameLoop;
	gameLoop.Init(window);
	//Model creation example ----------
	ModelComponentCreator mcc;
	//Load xml
	TiXmlDocument doc;
	doc.LoadFile("hello.xml");
	//scenenode
	//first model----
	TiXmlElement* elem = doc.FirstChildElement();
	ModelComponent* node = (ModelComponent*)mcc.CreateComponent(elem);

	//camera
	Camera* camera = new Camera(45.0f, 1.7778f, 0.1f, 10000.0f, Orbital_Mode, node);
	camera->VInitNode();
	GraphicsSystem::Get().GetScene()->SetCameraNode(shared_ptr<Camera>(camera));
	GraphicsSystem::Get().GetScene()->AddChild(shared_ptr<SceneNode>(node));
	gameLoop.StartLoop();
	return 0;
	// -----------------------------------------------------------------------
}
	
