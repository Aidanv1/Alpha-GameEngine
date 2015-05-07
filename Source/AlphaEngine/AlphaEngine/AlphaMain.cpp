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
	//node prop
	NodeProperties* nP = new NodeProperties();
	nP->m_lightVector = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	nP->m_rotationMatrix = rotate(mat4(1.0f), -1.57f, vec3(1.0f, 0.0f, 0.0f));
	nP->m_toWorld = translate(mat4(1.0f), vec3(0.0f, -2.0f, -4.0f));
	nP->m_toWorld = nP->m_toWorld*nP->m_rotationMatrix;
	node->SetNodeProperties(*nP);

	//camera
	Camera* camera = new Camera(45.0f, 1.7778f, 0.1f, 10000.0f);
	GraphicsSystem::Get().GetScene()->SetCameraNode(shared_ptr<Camera>(camera));
	GraphicsSystem::Get().GetScene()->AddChild(shared_ptr<SceneNode>(node));
	gameLoop.StartLoop();
	return 0;
	// -----------------------------------------------------------------------
}
	
