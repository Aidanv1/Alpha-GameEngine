#pragma once
#include "..\AlphaStd.h"
#include "IRenderer.h"
#include "..\ResourceManager\ResourceManager.h"
#include "..\ResourceManager\Loaders\BitmapResourceLoader.h"
#include "..\ResourceManager\Loaders\MeshResourceLoader.h"

typedef shared_ptr<Scene> StrongScenePtr;
//========================================================================
class GraphicsSystem
{
	typedef shared_ptr<IRenderer> StrongRendererPtr;
public:
	~GraphicsSystem();
	bool Init(StrongRendererPtr renderer, int textResSize, int meshResSize);
	StrongScenePtr GetScene() const { return m_scene; };
	void Update(float deltaMs);
	StrongResourceManagerPtr GetMeshResourceManager() { return m_meshResourceManager; }
	StrongResourceManagerPtr GetTextureResourceManager() { return m_textureResourceManager; }
	IRenderer* GetRenderer() { return m_renderer.get(); };
	void LoadScene();
	static GraphicsSystem& Get();
private:
	GraphicsSystem();
	StrongRendererPtr m_renderer;
	StrongResourceManagerPtr m_textureResourceManager;
	StrongResourceManagerPtr m_meshResourceManager;
	StrongScenePtr m_scene;
	bool m_Loaded;
};
//========================================================================
