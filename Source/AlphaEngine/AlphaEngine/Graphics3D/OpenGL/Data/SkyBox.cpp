#include "SkyBox.h"
SkyBox::SkyBox() :
SceneNode(),
GraphicsComponent(),
m_shaderProgram(NULL),
m_meshID(-1),
m_numVertices(36),
m_skyTexture(NULL),
m_vertexBuffer()
{
	m_nodeProperties.m_renderPass = RenderPass_Sky;
	m_textureFileName[0] = "";
	m_textureFileName[1] = "";
	m_textureFileName[2] = "";
	m_textureFileName[3] = "";
	m_textureFileName[4] = "";
	m_textureFileName[5] = "";
}
SkyBox::~SkyBox()
{

}
// -----------------------------------------------------------------------
bool SkyBox::VInitNode()
{
	bool success = true;
	if (Load() == -1)
	{
		success = false;
	}
	if (!LoadTexture())
	{
		success = false;
	}
	for (auto child = m_children.begin(); child != m_children.end(); child++)
	{
		if (!(*child)->VInitNode())
		{
			success = false;
		}
	}
	return success;
}
// -----------------------------------------------------------------------
void SkyBox::VRender(Scene* pScene)
{
	m_shaderProgram->VUseProgram();
	//get view and projection matrix from main camera
	mat4 viewMat;
	mat4 projMat;
	pScene->GetCamera()->GetViewMatrix(viewMat);
	vec3 pos;
	pScene->GetCamera()->GetPositionInWorld(pos);
	viewMat = translate(viewMat, -pos);

	pScene->GetCamera()->GetProjectionMatrix(projMat);
	//set shader uniforms
	mat4 M = projMat * viewMat;
	m_shaderProgram->SetUniforms(M,
		m_skyTexture->VGetTextureID());
	BindData();
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	VRenderChildren(pScene);
}
// -----------------------------------------------------------------------
int SkyBox::Load()
{
	if (m_vertexBuffer.GetID() != -1)
	{
		return m_vertexBuffer.GetID();
	}
	float vectarr[] = 
	{
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	return m_vertexBuffer.Init(m_numVertices, vectarr,3, m_skyName);
}
// -----------------------------------------------------------------------
bool SkyBox::LoadTexture()
{
	if (!m_skyTexture)
	{
		TextureCubeMap* tex = ALPHA_NEW TextureCubeMap(m_textureFileName);
		m_skyTexture = StrongTexturePtr(tex);
	}
	if (m_skyTexture->VLoadResource())
	{
		int textureID = m_skyTexture->VLoadTexture();
		if (textureID != -1)
		{
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------
void SkyBox::BindData()
{
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetPositionID(), 3, 0, 0);
}
//========================================================================
// IActorComponent Functions
//========================================================================
void SkyBox::VUpdate()
{

}
// -----------------------------------------------------------------------
bool SkyBox::VInitComponent(TiXmlElement* pElement)
{
	TiXmlElement* skyBoxElement = pElement->FirstChildElement();

	while (skyBoxElement)
	{
		string value = skyBoxElement->Value();
		if (value == "TextureCube")
		{
			m_textureFileName[0] = skyBoxElement->Attribute("left");
			m_textureFileName[1] = skyBoxElement->Attribute("right");
			m_textureFileName[2] = skyBoxElement->Attribute("top");
			m_textureFileName[3] = skyBoxElement->Attribute("bottom");
			m_textureFileName[4] = skyBoxElement->Attribute("front");
			m_textureFileName[5] = skyBoxElement->Attribute("back");
		}

		if (value == "Properties")
		{
			m_skyName = skyBoxElement->Attribute("name");

		}
		skyBoxElement = skyBoxElement->NextSiblingElement();
	}
	//init shader program
	SkyShaderProgram* pShaderProgram = dynamic_cast<GLRenderer*>(GraphicsSystem::Get().GetRenderer())->GetSkyShaderProgram();
	if (!pShaderProgram)
	{
		return false;
	}
	m_shaderProgram = pShaderProgram;
	return true;
}
// -----------------------------------------------------------------------
bool SkyBox::VPostInit()
{
	GraphicsComponent::VPostInit();
	return true;
}
