#include "GraphicsSystem.h"

GraphicsSystem::GraphicsSystem() :
m_renderer(NULL),
m_textureResourceManager(NULL),
m_meshResourceManager(NULL),
m_scene(NULL)
{

}
GraphicsSystem::~GraphicsSystem()
{
	
}
bool GraphicsSystem::Init(StrongRendererPtr renderer, int textResSize, int meshResSize)
{

	if (!renderer)
	{
		return false;
	}

	m_textureResourceManager = StrongResourceManagerPtr(new ResourceManager("TextureRM"));
	m_meshResourceManager = StrongResourceManagerPtr(new ResourceManager("MeshRM"));
	m_renderer = renderer;
	m_textureResourceManager->Init(textResSize);
	m_textureResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(new BitmapResourceLoader));
	m_meshResourceManager->Init(meshResSize);
	m_meshResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(new MeshResourceLoader));

	m_scene = shared_ptr<Scene>(new Scene());
	if (!m_scene->Init())
	{
		return false;
	}
	
	TiXmlDocument doc;
	if (!doc.LoadFile("Shaders/Shaders.xml"))
	{
		ALPHA_ERROR("Shaders.xml is missing");
		return false;
	}
	m_renderer->VInit(m_scene, doc.FirstChildElement());
	return true;
}
void GraphicsSystem::Update(float deltaMs)
{
	m_meshResourceManager->Update();
	m_textureResourceManager->Update();
	m_scene->Update(deltaMs);
	m_renderer->VRender();
}

GraphicsSystem& GraphicsSystem::Get()
{
	static GraphicsSystem* instance = new GraphicsSystem();
	return *instance;
}