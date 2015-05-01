#include "GLRenderer.h"
// -----------------------------------------------------------------------
GLRenderer::GLRenderer():
m_pScene(NULL),
m_backGroundColour(1)
{

}
// -----------------------------------------------------------------------
GLRenderer::~GLRenderer()
{
}
// -----------------------------------------------------------------------
bool GLRenderer::VInit(StrongScenePtr pScene)
{
	m_pScene = pScene;
	//shaderprogram
	m_meshShaderProgram = shared_ptr<MeshShaderProgram>(ALPHA_NEW MeshShaderProgram());
	if (!m_meshShaderProgram->VInit("Shaders/vertexshader.glsl", "Shaders/fragmentshader.glsl"))
	{
		return false;
	}

	return true;
}
// -----------------------------------------------------------------------
void GLRenderer::VRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_backGroundColour.x,
		m_backGroundColour.y,
		m_backGroundColour.z,
		m_backGroundColour.w);
	if (!m_pScene->GetRootNode())
	{
		ALPHA_ERROR("Render Error. NULL root scene node.");
		return;
	}
	m_pScene->GetRootNode()->VRender();
}
// -----------------------------------------------------------------------
void GLRenderer::VOnRestore()
{

}
// -----------------------------------------------------------------------
void GLRenderer::VSetBackGroundColour(vec4& backGroundColour)
{
	m_backGroundColour = backGroundColour;
}
// -----------------------------------------------------------------------