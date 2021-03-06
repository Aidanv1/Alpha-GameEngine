#include "..\AlphaStd.h"
#include "GraphicsSystem.h"
#include "..\ResourceManager\Loaders\BitmapResourceLoader.h"
#include "..\ResourceManager\Loaders\HeightMapResourceLoader.h"
#include "..\ResourceManager\Loaders\SimpleResourceLoader.h"
#include "GraphicsEvents.h"
#include "../Common/GameContext.h"
#include "Scene/Scene.h"
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
	m_scene = shared_ptr<Scene>(ALPHA_NEW Scene());
	if (!m_scene->Init(m_renderer.get()))
	{
		return false;
	}
	m_renderer = renderer;
	InitResource(textResSize, meshResSize);
	TiXmlDocument doc;

	string graphicsSettingsPath = GameContext::Get()->GetPath("Config");
	graphicsSettingsPath.append("GraphicsSettings.xml");
	string shaderPath = GameContext::Get()->GetPath("Shaders");
	shaderPath.append("Shaders.xml");
	if (!doc.LoadFile(shaderPath.c_str()))
	{
		ALPHA_ERROR("Shaders.xml is missing");
		return false;
	}
	m_renderer->VInit(doc.FirstChildElement());
	if (!doc.LoadFile(graphicsSettingsPath.c_str()))
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
	m_renderer->VRender(m_scene.get());
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
	Queue_Event(ALPHA_NEW GraphicsLoadedEvent());
}
// -----------------------------------------------------------------------
GraphicsSystem& GraphicsSystem::Get()
{
	static GraphicsSystem* instance = ALPHA_NEW GraphicsSystem();
	return *instance;
}
// -----------------------------------------------------------------------
bool GraphicsSystem::InitResource(int textResSize, int meshResSize)
{
	m_textureResourceManager = StrongResourceManagerPtr(ALPHA_NEW ResourceManager("TextureRM"));
	m_meshResourceManager = StrongResourceManagerPtr(ALPHA_NEW ResourceManager("MeshRM"));
	m_textureResourceManager->Init(textResSize);
	m_textureResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(ALPHA_NEW JPGResourceLoader()));
	m_textureResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(ALPHA_NEW PNGResourceLoader()));
	m_textureResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(ALPHA_NEW TGAResourceLoader()));
	m_textureResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(ALPHA_NEW BMPResourceLoader()));
	m_meshResourceManager->Init(meshResSize);
	m_meshResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(ALPHA_NEW HeightMapResourceLoader()));
	m_meshResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(ALPHA_NEW AMODResourceLoader()));
	return true;
}