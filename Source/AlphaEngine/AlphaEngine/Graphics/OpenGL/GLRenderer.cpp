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
bool GLRenderer::VInit(StrongScenePtr pScene, TiXmlElement* pElement)
{
	m_pScene = pScene;
	bool success = true;
	TiXmlElement* pShaderProgram = pElement->FirstChildElement();

	while (pShaderProgram != NULL)
	{
		string value = pShaderProgram->Attribute("ShaderType");
		if (value == "Mesh")
		{
			m_meshShaderProgram = shared_ptr<MeshShaderProgram>(ALPHA_NEW MeshShaderProgram());
			const char* vertexShaderName = pShaderProgram->Attribute("vertexShaderFile");
			const char* fragmentShaderName = pShaderProgram->Attribute("fragmentShaderFile");

			if (!m_meshShaderProgram->VInit(vertexShaderName, fragmentShaderName))
			{
				success = false;
			}
		}
		if (value == "Text2D")
		{
			m_text2DShaderProgram = shared_ptr<Text2DShaderProgram>(ALPHA_NEW Text2DShaderProgram());
			const char* vertexShaderName = pShaderProgram->Attribute("vertexShaderFile");
			const char* fragmentShaderName = pShaderProgram->Attribute("fragmentShaderFile");

			if (!m_text2DShaderProgram->VInit(vertexShaderName, fragmentShaderName))
			{
				success = false;
			}
		}
		pShaderProgram = pShaderProgram->NextSiblingElement();
	}					
	// ---------------------------------
	//basic text renderer for dev
	Text2D_GL* t2d = new Text2D_GL();
	t2d->VInitText2D("../../../Assets/fonts/dev_font.png", 10);	
	GraphicsSystem::Get().GetScene()->AddChild(shared_ptr<SceneNode>(t2d));
	m_text2DNode = shared_ptr<IText2D>(t2d);

	if (!m_meshShaderProgram)
	{
		success = false;
	}
	if (!m_text2DShaderProgram)
	{
		success = false;
	}
	return success;
}
// -----------------------------------------------------------------------
void GLRenderer::VRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_backGroundColour.x,
		m_backGroundColour.y,
		m_backGroundColour.z,
		m_backGroundColour.w);

	m_pScene->Render();
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
void GLRenderer::VPrintText(string text)
{
	m_text2DNode->VPrintText2D(text,0,0,0.06);
}

// -----------------------------------------------------------------------
