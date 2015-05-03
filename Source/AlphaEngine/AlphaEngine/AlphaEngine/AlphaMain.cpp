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

int main(int argc, char *argv[])
{
	IWindow* window = ALPHA_NEW GLWindow("AlphaEngine", 1280, 720);
	GameLoop gameLoop;
	gameLoop.Init(window);
	ModelComponentCreator mcc;
	TiXmlDocument doc;
	doc.LoadFile("hello.xml");
	//scenenode
	//first model----
	TiXmlElement* elem = doc.FirstChildElement();
	ModelComponent* node = (ModelComponent*)mcc.CreateComponent(elem);
	//node prop
	NodeProperties* nP = new NodeProperties();
	nP->LightVector = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	nP->RotationMatrix = rotate(mat4(1.0f), -1.57f, vec3(1.0f, 0.0f, 0.0f));
	nP->ModelMatrix = translate(mat4(1.0f), vec3(0.0f, -6.0f, -4.0f));
	nP->ModelMatrix = nP->ModelMatrix*nP->RotationMatrix;
	nP->ViewMatrix = rotate(mat4(1.0f), 0.5f, vec3(1.0f, 0.0f, 0.0f));
	nP->ProjectionMatrix = perspective(45.0f, 1.7778f, 0.1f, 10000.0f);
	node->SetNodeProperties(*nP);
	//second model----
	elem = elem->NextSiblingElement();
	ModelComponent* node2 = (ModelComponent*)mcc.CreateComponent(elem);
	//node prop
	NodeProperties* nP2 = new NodeProperties();
	nP2->LightVector = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	nP2->RotationMatrix = rotate(mat4(1.0f), 0.0f, vec3(0.0f, 1.0f, 0.0f));
	nP2->ModelMatrix = translate(mat4(1.0f), vec3(-6.0f, -6.0f, -10.0f));
	nP2->ModelMatrix = nP2->ModelMatrix*nP2->RotationMatrix;
	nP2->ViewMatrix = rotate(mat4(1.0f), 0.5f, vec3(1.0f, 0.0f, 0.0f));
	nP2->ProjectionMatrix = perspective(45.0f, 1.7778f, 0.1f, 10000.0f);
	node2->SetNodeProperties(*nP2);
	//second model----
	elem = elem->NextSiblingElement();
	ModelComponent* node3 = (ModelComponent*)mcc.CreateComponent(elem);
	//node prop
	NodeProperties* nP3 = new NodeProperties();
	nP3->LightVector = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	nP3->RotationMatrix = rotate(mat4(1.0f), -1.57f, vec3(1.0f, 0.0f, 0.0f));
	nP3->ModelMatrix = translate(mat4(1.0f), vec3(6.0f, -6.0f, -10.0f));
	nP3->ModelMatrix = nP3->ModelMatrix*nP3->RotationMatrix;
	nP3->ViewMatrix = rotate(mat4(1.0f), 0.5f, vec3(1.0f, 0.0f, 0.0f));
	nP3->ProjectionMatrix = perspective(45.0f, 1.7778f, 0.1f, 10000.0f);
	node3->SetNodeProperties(*nP3);

	node->VAddChild(StrongSceneNodePtr(node2));
	node->VAddChild(StrongSceneNodePtr(node3));

	GraphicsSystem::Get().GetScene()->SetRootNode(shared_ptr<SceneNode>(node));
	gameLoop.StartLoop();

	return 0;
	// -----------------------------------------------------------------------
}
	
