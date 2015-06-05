#include "Renderer_GL.h"
#include "../GraphicsSettings.h"
// -----------------------------------------------------------------------
Renderer_GL::Renderer_GL():
m_backGroundColour(1,1,1,1)
{

}
// -----------------------------------------------------------------------
Renderer_GL::~Renderer_GL()
{
}
// -----------------------------------------------------------------------
bool Renderer_GL::VInit(TiXmlElement* pElement)
{
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
		if (value == "Sky")
		{
			m_skyShaderProgram = shared_ptr<SkyShaderProgram>(ALPHA_NEW SkyShaderProgram());
			const char* vertexShaderName = pShaderProgram->Attribute("vertexShaderFile");
			const char* fragmentShaderName = pShaderProgram->Attribute("fragmentShaderFile");

			if (!m_skyShaderProgram->VInit(vertexShaderName, fragmentShaderName))
			{
				success = false;
			}
		}
		if (value == "HeightMap")
		{
			m_heightMapShaderProgram = shared_ptr<HeightMapShaderProgram>(ALPHA_NEW HeightMapShaderProgram());
			const char* vertexShaderName = pShaderProgram->Attribute("vertexShaderFile");
			const char* fragmentShaderName = pShaderProgram->Attribute("fragmentShaderFile");

			if (!m_heightMapShaderProgram->VInit(vertexShaderName, fragmentShaderName))
			{
				success = false;
			}
		}
		if (value == "Basic")
		{
			m_basicShaderProgram = shared_ptr<BasicShaderProgram>(ALPHA_NEW BasicShaderProgram());
			const char* vertexShaderName = pShaderProgram->Attribute("vertexShaderFile");
			const char* fragmentShaderName = pShaderProgram->Attribute("fragmentShaderFile");

			if (!m_basicShaderProgram->VInit(vertexShaderName, fragmentShaderName))
			{
				success = false;
			}
		}

		pShaderProgram = pShaderProgram->NextSiblingElement();
	}		
	ALPHA_ASSERT(m_meshShaderProgram);
	ALPHA_ASSERT(m_text2DShaderProgram);
	ALPHA_ASSERT(m_skyShaderProgram);
	ALPHA_ASSERT(m_heightMapShaderProgram);
	ALPHA_ASSERT(m_basicShaderProgram);

	if (!m_meshShaderProgram)
	{
		success = false;
	}
	if (!m_text2DShaderProgram)
	{
		success = false;
	}

	if (!m_skyShaderProgram)
	{
		success = false;
	}

	//opengl render settings
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	return success;
}
// -----------------------------------------------------------------------
void Renderer_GL::VRender(StrongScenePtr scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_backGroundColour.x,
		m_backGroundColour.y,
		m_backGroundColour.z,
		m_backGroundColour.w);
	//WireFrame mode
	if (GraphicsSettings::WireFrame())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	scene->Render();
}
// -----------------------------------------------------------------------
void Renderer_GL::VOnRestore()
{
	
}
// -----------------------------------------------------------------------
void Renderer_GL::VSetBackGroundColour(vec4& backGroundColour)
{
	m_backGroundColour = backGroundColour;
}
// -----------------------------------------------------------------------
void Renderer_GL::VDepthBuffer(DepthBufferCommand depthMode)
{
	switch (depthMode)
	{
	case DepthBufferDisabled:
		glDisable(GL_DEPTH_TEST);
		break;
	case DepthBufferEnabled:
		glEnable(GL_DEPTH_TEST);
		break;
	case DepthBufferLess:
		glDepthFunc(GL_LESS);
		break;
	case DepthBufferEqual:
		glDepthFunc(GL_EQUAL);
		break;
	case DepthBufferAlways:
		glDepthFunc(GL_ALWAYS);
		break;
	}
}
// -----------------------------------------------------------------------