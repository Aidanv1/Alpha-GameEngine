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
	TiXmlElement* pGraphicsElement = pElement->FirstChildElement();
	while (pGraphicsElement != NULL)
	{
		string value = pGraphicsElement->Value();
		//ShaderProgramList-------------------
		if (value == "ShaderProgramList")
		{
			TiXmlElement* pShaderProgram = pGraphicsElement->FirstChildElement();

			while (pShaderProgram != NULL)
			{
				value = pShaderProgram->Attribute("ShaderType");
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
				pShaderProgram = pShaderProgram->NextSiblingElement();
			}			
		}
		// ---------------------------------
		pGraphicsElement = pGraphicsElement->NextSiblingElement();
	}
	
	if (!m_meshShaderProgram)
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