#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>	
#include <glm/gtx/transform.hpp>
#include "Graphics\OpenGL\Data\Texture.h"
#include "ResourceManager\Loaders\BitmapResourceLoader.h"
#include "ResourceManager\Loaders\MeshResourceLoader.h"
#include "Graphics\GraphicsSystem.h"
#include "Graphics\OpenGL\Data\Mesh.h"
#include "Graphics\OpenGL\Data\Material.h"
#include "Graphics\OpenGL\Data\Texture.h"
#include "GameLoop\GameLoop.h"

int main(int argc, char *argv[])
{

	GameLoop gameLoop;
	gameLoop.Init();

	//material	
	Material* mat = new Material;
	Texture* tex = new Texture("../../../../Assets/mirage.bmp", GraphicsSystem::Get().GetTextureResourceManager());
	shared_ptr<Texture> ptex(tex);
	mat->SetTexture(ptex);
	//scenenode
	SceneNode* node = new Mesh("../../../../Assets/mirage.obj", shared_ptr<Material>(mat), GraphicsSystem::Get().GetMeshResourceManager());
	node->VLoad();
	node->VInit();
	//node prop
	NodeProperties* nP = new NodeProperties();
	nP->LightVector = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	nP->RotationMatrix = rotate(mat4(1.0f), 3.0f, vec3(0.0f, 1.0f, 0.0f));
	nP->ModelMatrix = translate(mat4(1.0f), vec3(0.0f, -10.0f, -20.0f));
	nP->ModelMatrix = nP->ModelMatrix*nP->RotationMatrix;
	nP->ViewMatrix = rotate(mat4(1.0f), 0.0f, vec3(0.0f, 1.0f, 0.0f));
	nP->ProjectionMatrix = perspective(45.0f, 1.7778f, 0.1f, 1000.0f);
	node->SetNodeProperties(*nP);
	GraphicsSystem::Get().GetScene()->SetRootNode(shared_ptr<SceneNode>(node));
	gameLoop.StartLoop();

	return 0;
	// -----------------------------------------------------------------------
}
	
