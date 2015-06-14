#include "GraphicsSystem.h"
// -----------------------------------------------------------------------
GraphicsSystem::GraphicsSystem() :
m_renderer(NULL),
m_textureResourceManager(NULL),
m_meshResourceManager(NULL),
m_scene(NULL),
m_Loaded(false)
{

}
// -----------------------------------------------------------------------
GraphicsSystem::~GraphicsSystem()
{
	
}
// -----------------------------------------------------------------------
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
	if (!m_scene->Init(m_renderer.get()))
	{
		return false;
	}
	
	TiXmlDocument doc;
	if (!doc.LoadFile("Shaders/Shaders.xml"))
	{
		ALPHA_ERROR("Shaders.xml is missing");
		return false;
	}
	m_renderer->VInit(doc.FirstChildElement());
	if (!doc.LoadFile("GraphicsSettings.xml"))
	{
		ALPHA_ERROR("GraphicsSettings.xml is missing");
		return false;
	}
	GraphicsSettings::Init(doc.FirstChildElement());
	return true;
}
// -----------------------------------------------------------------------
void GraphicsSystem::Update(float deltaMs)
{
	m_scene->Update(deltaMs);
}
// -----------------------------------------------------------------------
void GraphicsSystem::Render(float deltaMs)
{
	m_renderer->VRender(m_scene);
}
// -----------------------------------------------------------------------
void GraphicsSystem::LoadScene()
{
	int passCount = 0;
	//contiuously call recursive init function until it returns true

	do
	{
		m_meshResourceManager->Update();
		m_textureResourceManager->Update();
		m_scene->Update(0);
		passCount++;
		if (passCount > 5)
		{
			ALPHA_ERROR("Scene Load pass limit exceeded.");
		}
	} while (!m_scene->Load());
	m_Loaded = true;
}
// -----------------------------------------------------------------------
GraphicsSystem& GraphicsSystem::Get()
{
	static GraphicsSystem* instance = ALPHA_NEW GraphicsSystem();
	return *instance;
}