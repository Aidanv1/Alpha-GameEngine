#include "Time\Clock.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>	
#include <glm/gtx/transform.hpp>//openGL maths class
#include "Window\GLWindow.h"
#include "Graphics\OpenGL\Data\Texture.h"
#include "ResourceManager\Loaders\BitmapResourceLoader.h"
#include "ResourceManager\Loaders\MeshResourceLoader.h"
#include "Graphics\GraphicsSystem.h"
#include "Graphics\OpenGL\GLRenderer.h"
#include "Graphics\OpenGL\Data\Mesh.h"
#include "Graphics\OpenGL\Data\Material.h"
#include "Graphics\OpenGL\Data\Texture.h"

#define WINDOW_NAME "Alpha Engine"
int main(int argc, char *argv[])
{
	//Window
	GLWindow glwin(WINDOW_NAME, 1280, 720);
	glwin.VInit();
	//Renderer
	IRenderer* prend = new GLRenderer();
	shared_ptr<IRenderer> renderer(prend);
	//graphics system
	GraphicsSystem::Get().Init(renderer, 100, 100);
	//--------------------------------------------------------------

	//scenenode
	SceneNode* node = new SceneNode();
	//drawable	
	Material* mat = new Material;
	Texture* tex = new Texture("../../../../Assets/mirage.bmp", GraphicsSystem::Get().GetTextureResourceManager());
	shared_ptr<Texture> ptex(tex);
	mat->SetTexture(ptex);

	IDrawable* drawable = new Mesh("../../../../Assets/mirage.obj", shared_ptr<Material>(mat), GraphicsSystem::Get().GetMeshResourceManager());
	shared_ptr<IDrawable> pd(drawable);
	pd->VLoad();
	pd->VInit(node);
	
	node->Init(pd);
	//node prop
	NodeProperties nP;
	nP.LightVector = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	nP.RotationMatrix = mat4(1.0f);
	nP.ModelMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -20.0f));
	nP.ViewMatrix = mat4(1.0f);
	nP.ProjectionMatrix = perspective(45.0f, 1.7778f, 0.1f, 1000.0f);

	node->SetNodeProperties(nP);
	GraphicsSystem::Get().GetScene()->SetRootNode(shared_ptr<SceneNode>(node));




	//Game Loop
	bool quit = false;
	while (!quit)
	{
		GraphicsSystem::Get().Update();
		quit = !glwin.VUpdate();
	}
	return 0;
}
	

